# Groonga snippet_tritonn function 
  It's groonga customized snippet function like tritton's snippet API.

# Example

	register function/snippet_tritonn
	table_create Entries TABLE_NO_KEY
	column_create Entries title COLUMN_SCALAR ShortText
	column_create Entries content COLUMN_SCALAR ShortText
	
	table_create Tokens TABLE_PAT_KEY ShortText --default_tokenizer TokenBigram
	column_create Tokens entries_title COLUMN_INDEX|WITH_POSITION Entries title
	column_create Tokens entries_content COLUMN_INDEX|WITH_POSITION Entries content
	
	load --table Entries
	[
	{"title": "groonga and MySQL", "content": "groonga + MySQL = mroonga."}
	]
	
	select Entries \
	--output_columns 'title, snippet_tritonn(title, 200, 2, "NormalizerAuto", 1, 1, "...", "...", \
	"groonga", "<span class=\\"w1\\">", "</span>", "mysql", "<span class=\\"w2\\">", "</span>"), content' \
	--command_version 2 \
	--match_columns 'title' \
	--query groonga

## Install

Install libgroonga-dev

Build this function.

    % sh autogen.sh
    % ./configure
    % make
    % sudo make install

## License

LGPL 2.1. See COPYING for details.

This plugin based on the original groonga snippet function.

<https://github.com/groonga/groonga/blob/master/lib/proc.c>
