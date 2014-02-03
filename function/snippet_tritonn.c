/* -*- c-basic-offset: 2 -*- */
/*
  Copyright(C) 2009-2014 Brazil

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <groonga/plugin.h>

#include <string.h>
#include <stdlib.h>

#ifdef __GNUC__
# define GNUC_UNUSED __attribute__((__unused__))
#else
# define GNUC_UNUSED
#endif


static grn_obj *
func_snippet_tritonn(grn_ctx *ctx, int nargs, grn_obj **args,
                  grn_user_data *user_data)
{
  grn_obj *snippets = NULL;

  if (nargs > 10) {
    grn_obj *text = args[0];
    grn_obj *snip = NULL;
    unsigned int width = GRN_UINT64_VALUE(args[1]);
    unsigned int max_n_results = GRN_UINT64_VALUE(args[2]);
    grn_snip_mapping *mapping = NULL;

    int flags = GRN_SNIP_COPY_TAG;

    if(GRN_UINT64_VALUE(args[4])){
      flags |= GRN_SNIP_SKIP_LEADING_SPACES;
    }
    if(GRN_UINT64_VALUE(args[5])){
      mapping = GRN_SNIP_MAPPING_HTML_ESCAPE;
    }
    snip = grn_snip_open(ctx, flags, width, max_n_results, "", 0, "", 0, mapping);

    if (snip) {
      grn_rc rc;
      unsigned int i,n_results,max_tagged_length;
      grn_obj snippet_buffer;

      if(GRN_TEXT_LEN(args[3])){
        grn_obj * normalizer;
        normalizer = grn_ctx_get(ctx, GRN_TEXT_VALUE(args[3]), GRN_TEXT_LEN(args[3]));
        grn_snip_set_normalizer(ctx, snip, normalizer); 
      }

      for(i = 8; i < (unsigned int)nargs; i += 3){
        rc = grn_snip_add_cond(ctx, snip,
                               GRN_TEXT_VALUE(args[i]), GRN_TEXT_LEN(args[i]),
                               GRN_TEXT_VALUE(args[i + 1]), GRN_TEXT_LEN(args[i + 1]),
                               GRN_TEXT_VALUE(args[i + 2]), GRN_TEXT_LEN(args[i + 2]));
      }

      rc = grn_snip_exec(ctx, snip,
                         GRN_TEXT_VALUE(text), GRN_TEXT_LEN(text),
                         &n_results, &max_tagged_length);
      if (rc != GRN_SUCCESS) {
        GRN_OBJ_FIN(ctx, &snippet_buffer);
        grn_snip_close(ctx, snip);
        return NULL;
      }

      if (n_results == 0) {
        snippets = grn_plugin_proc_alloc(ctx, user_data, GRN_DB_VOID, 0);
      }
      else{
        snippets = grn_plugin_proc_alloc(ctx, user_data, GRN_DB_SHORT_TEXT, GRN_OBJ_VECTOR);
        if (!snippets) {
          GRN_OBJ_FIN(ctx, &snippet_buffer);
          grn_snip_close(ctx, snip);
          return NULL;
        }
      }

      GRN_TEXT_INIT(&snippet_buffer, 0);
      grn_bulk_space(ctx, &snippet_buffer, GRN_TEXT_LEN(args[6]) + max_tagged_length + GRN_TEXT_LEN(args[7]));
      for (i = 0; i < n_results; i++) {
        unsigned int snippet_length;

        GRN_BULK_REWIND(&snippet_buffer);
        GRN_TEXT_PUT(ctx,&snippet_buffer,GRN_TEXT_VALUE(args[6]),GRN_TEXT_LEN(args[6]));
        rc = grn_snip_get_result(ctx, snip, i,
                                 GRN_TEXT_VALUE(&snippet_buffer) + GRN_TEXT_LEN(args[6]),
                                 &snippet_length);
        grn_vector_add_element(ctx, snippets,
                               strcat(GRN_TEXT_VALUE(&snippet_buffer), GRN_TEXT_VALUE(args[7])),
                               GRN_TEXT_LEN(args[6]) + snippet_length + GRN_TEXT_LEN(args[7]),
                               0, GRN_DB_SHORT_TEXT);
      }
      GRN_OBJ_FIN(ctx, &snippet_buffer);
      grn_obj_close(ctx, snip);
    }
  }
  if(snippets == NULL){
    snippets = grn_plugin_proc_alloc(ctx, user_data, GRN_DB_VOID, 0);
  }
  return snippets;
}


grn_rc
GRN_PLUGIN_INIT(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  grn_proc_create(ctx, "snippet_tritonn", -1, GRN_PROC_FUNCTION,
                  func_snippet_tritonn, NULL, NULL, 0, NULL);

  return GRN_SUCCESS;
}

grn_rc
GRN_PLUGIN_FIN(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}
