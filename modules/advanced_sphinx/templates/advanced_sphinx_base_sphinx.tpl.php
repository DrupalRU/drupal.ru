source source_main
{
  type               = mysql
  sql_host           = <?php print $config['sql_host']; ?>
  sql_user           = <?php print $config['sql_user']; ?>
  sql_pass           = <?php print $config['sql_pass']; ?>
  sql_db             = <?php print $config['sql_db']; ?>
  sql_port           = 3306
  sql_query_pre      = SET NAMES utf8
  sql_query_pre      = SET CHARACTER SET utf8
  sql_query_pre      = <?php print $config['sql_query_pre_source_main']; ?>
  sql_query          = <?php print $config['sql_query_source_main']; ?>
  sql_attr_uint      = node_id
  sql_attr_uint      = countitl
  sql_attr_timestamp = created
  sql_attr_timestamp = changed
  sql_query_info     = <?php print $config['sql_query_info']; ?>
}

source source_delta : source_main
{ sql_query_pre      = SET NAMES utf8
  sql_query_pre      = SET CHARACTER SET utf8
  sql_query          = <?php print $config['sql_query_source_delta']; ?>
  sql_attr_uint      = node_id
  sql_attr_uint      = countitl
  sql_attr_timestamp = created
  sql_attr_timestamp = changed
}

index index_main
{
  source			 = source_main
  path				 = <?php print $config['index_main']; ?>
  docinfo			 = extern
  morphology		 = stem_ru, stem_en
  charset_type		 = utf-8
  charset_table		 = <?php print $config['charset_table']; ?>
  min_word_len		 = 1
  html_strip		 = 1
}

index index_delta : index_main
{
  source			 = source_delta
  path				 = <?php print $config['index_delta']; ?>
}

indexer
{
  mem_limit			 = 64M
}

searchd
{
  listen             = <?php print $config['listen']; ?>
  log				 = <?php print $config['log']; ?>
  query_log		     = <?php print $config['query_log']; ?>
  read_timeout	     = 5
  max_children	     = 30
  pid_file		     = <?php print $config['searchd']; ?>
  max_matches		 = 1000
  seamless_rotate	 = 1
  preopen_indexes	 = 1
  unlink_old		 = 1
}
