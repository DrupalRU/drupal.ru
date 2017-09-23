Alternative Pager
========

Alternative Pager. API for alternative pager. It is alternative view point on
Pager functionality


Example code or module "altpager_example":

```php
<?php

$query = db_select('node', 'n')->extend('AltPager');
$nids = $query
  ->fields('n', array('nid', 'sticky', 'created'))
  ->execute()
  ->fetchAll();
$pager = theme('altpager');

$result = $pager;

foreach ($nids as $row) {
  $node = node_load($row->nid);
  $result .= render(node_view($node));
}

$result .= theme('altpager');

echo $result;
```
Output is:
------
![alt tag](http://i59.tinypic.com/55ou95.png)