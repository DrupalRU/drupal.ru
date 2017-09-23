<?php

class PearwikiFilter {
  protected $name;
  protected $settings;
  protected $pearOverridePath;

  protected function __construct($settings, $name) {
    $this->name = $name;
    $this->settings = $settings;
  }

  /**
   * Creates parser instance of specified syntax.
   *
   * @param array $settings
   * @param string $name
   *
   * @return PearwikiFilter
   */
  public static function get($settings, $name = '') {
    if (empty($name) && !empty($settings['syntax'])) {
      $name = $settings['syntax'];
    }

    $class_name = 'PearwikiFilter' . $name;
    if (class_exists($class_name)) {
      return new $class_name($settings, $name);
    }
    else {
      return new self($settings, $name);
    }
  }

  /**
   * List of available syntaxes.
   */
  public function syntaxes() {
    $result = array();
    $dirname = $this->pearPath() . '/Text/Wiki';
    if (is_dir($dirname) && ($handle = opendir($dirname))) {
      while ($file = readdir($handle)) {
        if ($file != 'Render.php' && $file != 'Parse.php' && is_file($dirname .'/'. $file)) {
          $name = basename($file, '.php');
          $processor = self::get(array(), $name);
          $result[$processor->name()] = $processor->label();
        }
      }
      closedir($handle);
    }
    return $result;
  }

  /**
   * Returns settings form definition.
   *
   * @return array
   */
  public function settingsForm() {
    $settings = $this->settings;

    $form = array();

    $syntaxes = $this->syntaxes();
    if (count($syntaxes)) {
      $form['syntax'] = array(
        '#type' => 'radios',
        '#title' => t('Format'),
        '#default_value' => isset($settings['syntax']) ? $settings['syntax'] : '',
        '#options' => $syntaxes,
        '#description' => t('Select the Wikiformat to use. Click on a name to view an overview of the syntax.'),
      );
    }

    // Compatibility options.
    $form['compatibility'] = array(
      '#type' => 'fieldset',
      '#title' => 'Compatibility',
      '#description' => t('The following options control ... .'),
    );

    $form['compatibility']['allow_html'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow HTML?'),
      '#default_value' => isset($settings['allow_html']) ? $settings['allow_html'] : '',
      '#description' => t('Allow HTML tags to be used.'),
    );

    $form['compatibility']['ignore_tags'] = array(
      '#type' => 'textfield',
      '#title' => t('Ignore tags'),
      '#default_value' => isset($settings['ignore_tags']) ? $settings['ignore_tags'] : '',
      '#description' => t('Tags which are fully ignored (the tag, attributes and content is untouched). Use this for compatibility with other filters. Specify a space-separated list of tagnames.<br/>Example: If you specify <em>code</em> than the filter ignores &lt;code type="php"&gt; ... &lt;/code&gt; which can then be processed by other filters like the <a href="http://drupal.tschannen.net/wiki/geshi_filter">GeSHi filter</a>. This can introduce a security risk through XSS scripting. Make sure the ignored text will be processed by another filter.')
    );

    $form['compatibility']['ignore_regexp'] = array(
      '#type' => 'textfield',
      '#title' => t('Ignore regexp'),
      '#default_value' => isset($settings['ignore_regexp']) ? $settings['ignore_regexp'] : '',
      '#description' => t('A regular expression where the match is ignored (the full match is untouched). Use this for compatibility with other filters. This can introduce a security risk through XSS scripting. Make sure the ignored text will be processed by another filter.')
    );

    // Wikilinks options.
    $form['wikilinks'] = array(
      '#type' => 'fieldset',
      '#title' => t('Wikilinks'),
      '#description' => t('The following options control the behavior when wikilinks are used.')
    );

    $form['wikilinks']['use_wiki_links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use wiki links'),
      '#default_value' => isset($settings['use_wiki_links']) ? $settings['use_wiki_links'] : '',
      '#description' => t('Are wiki links activated? If this option is not activated, wikilinks will not be parsed.')
    );

    $form['wikilinks']['wikilinks_plugin'] = array(
      '#type' => 'select',
      '#title' => t('Plugin'),
      '#required' => TRUE,
      '#default_value' => $settings['wikilinks_plugin'] ? : 'internal',
      '#options' => array('internal' => t('Internal')),
      '#states' => array(
        'invisible' => array(':input[name="filters[pearwiki_filter_wiki][settings][wikilinks][use_wiki_links]"]' => array('checked' => FALSE)),
      ),
      '#description' => "<ul>
        <li><strong>" . t('Internal') . '</strong>: '. t('All links are processed by the pearwiki_filter module itself. If a node with the same title was found, link will be created, otherwise plain text will be rendered.') . "</li>
        <li>" . l('Wikitools', 'http://drupal.org/project/wikitools') . ': ' . t('All links are passed to the wikitools module for handling and have the wikipath prepended which is specified in the wikitools settings. The module wikitools has to be enabled.') . "</li>
        <li>" . l('Freelinking', 'http://drupal.org/project/freelinking') . ': ' . t('All links are passed to the freelinking module for handling, thus they have the form \'freelinking/Page Title\'. The module freelinking has to be enabled, but you don\'t have to activate the freelinking filter for this format.') . "</li>
      </ul>",
    );

    $node_types = array();
    foreach (node_type_get_types() as $type) {
      $node_types[$type->type] = $type->name;
    }

    $form['wikilinks']['wikilinks_content_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Link to content type'),
      '#default_value' => $settings['wikilinks_content_types'] ? : array(),
      '#options' => $node_types,
      '#states' => array(
        'invisible' => array(':input[name="filters[pearwiki_filter_wiki][settings][wikilinks][use_wiki_links]"]' => array('checked' => FALSE)),
      ),
    );

    if (module_exists('wikitools')) {
      $form['wikilinks']['wikilinks_plugin']['#options']['wikitools'] = 'Wikitools';
    }

    if (module_exists('freelinking')) {
      $form['wikilinks']['wikilinks_plugin']['#options']['freelinking'] = 'Freelinking';
    }

    $form['wikilinks']['interwiki_mapping'] = array(
      '#type' => 'textarea',
      '#title' => t('Interwiki Mapping'),
      '#default_value' => isset($settings['interwiki_mapping']) ? $settings['interwiki_mapping'] : '',
      '#description' => t('A list of interwiki mappings Each line denotes one mapping. The format is: <em>title</em> | <em>interwiki name</em> | <em>mapping url</em>. <br />&lt;path&gt; is a special mapping url which will be transformed into a Drupal path for the current site.'),
      '#states' => array(
        'invisible' => array(':input[name="filters[pearwiki_filter_wiki][settings][wikilinks][use_wiki_links]"]' => array('checked' => FALSE)),
      ),
    );

    // Image options.
    $form['images'] = array(
      '#type' => 'fieldset',
      '#title' => 'Images',
      '#description' => t('The following options control the behavior when image links are used.')
    );

    $form['images']['image_base'] = array(
      '#type' => 'textfield',
      '#title' => t('Base path for images'),
      '#default_value' => isset($settings['image_base']) ? $settings['image_base'] : '',
      '#description' => t('Base path for images. Images will be looked for in this directory.')
    );

    return $form;
  }

  /**
   * Returns machine syntax name.
   *
   * @return string
   *   Raw string.
   */
  public function name() {
    return $this->name;
  }

  /**
   * Returns human-readable syntax name.
   *
   * @return string
   *   Raw string.
   */
  public function title() {
    return $this->name;
  }

  /**
   * Returns syntax documentation URL (if any).
   *
   * @return string
   */
  protected function syntaxUrl() {
    return '';
  }

  /**
   * Returns form element label for current syntax.
   *
   * @return string
   */
  public function label() {
    if ($url = $this->syntaxUrl()) {
      return l($this->title(), $this->syntaxUrl());
    }
    else {
      return check_plain($this->name());
    }
  }

  /**
   * Returns filter tips.
   *
   * @param bool $long
   *
   * @return string
   */
  public function tips($long = FALSE) {
    $syntax_url = $this->syntaxUrl();
    if ($syntax_url) {
      // An url to the wiki syntax was provided.
      $output = '<p>' . t('You can use <a href="!url">%wiki syntax</a>. It is possible that not all formatting options are supported at the moment.', array(
        '!url' => $syntax_url,
        '%wiki' => $this->title(),
      )) . '</p>';
    }
    else {
      // Default help text.
      $output = '<p>' . t('You can use %wiki syntax. It is possible that not all formatting options are supported at the moment.', array(
        '%wiki' => $this->title(),
      )) . '</p>';
      $output .= $this->interwikiTips($long);
    }
    return $output;
  }

  // TODO check for XSS and text format
  public function interwikiTips($long = FALSE) {
    $output = '';
    $mappings = $this->interwikiMappings('array');
    if (count($mappings)) {
      if ($long) {
        $output .= '<p>'. t('You can use the following interwiki links:') .'</p>';
        $output .= '<ul>';
        foreach ($mappings as $key => $info) {
          // TODO move into t()
          $output .= '<li>'. $key .': '. $info['title'] . t(' - mapped to %url', array('%url' => $info['url'])) .'</li>';
        }
        $output .= '</ul>';
      }
      else {
        $output .= t('You can use the following interwiki links: ');
        $output .= implode(', ', array_keys($mappings));
      }
    }
    return $output;
  }

  protected function pearPath() {
    return libraries_get_path('Text_Wiki');
  }

  protected function modifyIncludePath() {
    static $paths = array();
    $path = $this->pearPath();
    if (!isset($paths[$path])) {
      ini_set('include_path', realpath($path) . PATH_SEPARATOR . ini_get('include_path'));
      $paths[$path] = TRUE;
    }
  }

  // TODO do we really need realpath(), getcwd() and value cache here ?
  protected function pearOverridePath() {
    if (!isset($this->pearOverridePath)) {
      $this->pearOverridePath = realpath(getcwd() . '/' . drupal_get_path('module', 'pearwiki_filter') . '/pear_override');
    }
    return $this->pearOverridePath;
  }

  protected function configureWiki(&$wiki) {
    // Do nothing by default.
  }

  /**
   * Returns information about interwiki mappings.
   *
   * @param array $settings
   * @param string $op
   *   When 'url' is passed, returns an array with interwiki ids as key and url
   *   mapping as values. When 'array' is passed, returns an array with interwiki
   *   ids as key and an associative array with 'id', 'title' and 'url' as values.
   *
   * @todo refactor
   */
  protected function interwikiMappings($op = 'url') {
    $mappings = array();
    // TODO move exploding to settings save if possible.
    $lines = explode("\n", $this->settings['interwiki_mapping']);
    foreach ($lines as $line) {
      if ($line) {
        list($title, $id, $url) = explode('|', $line, 3);
        if (trim($url) == '<path>') {
          $url = str_replace('__QUERY_POSITION__', '%s', url('__QUERY_POSITION__'));
        }
        if ($op == 'url') {
          $mappings[trim($id)] = trim($url);
        }
        elseif ($op == 'array') {
          $mappings[trim($id)] = array(
            'id' => trim($id),
            'title' => trim($title),
            'url' => trim($url),
          );
        }
      }
    }
    return $mappings;
  }

  public function process($text) {
    // Load PEAR files if necessary.
    $this->modifyIncludePath();
    if (!class_exists('Text_Wiki', FALSE)) {
      @include_once 'Text/Wiki.php';
    }
    if (!class_exists('Text_Wiki', FALSE)) {
      // Loading of PEAR class failed.
      drupal_set_message(t('Loading of <em>Text_Wiki</em> class from PEAR failed. Please, contact website administrator.'), 'error');
      // TODO watchdog error
      return '<pre>'. check_plain($text) .'</pre>';
    }

    $syntax = $this->settings['syntax'];
    $wiki =& Text_Wiki::singleton($syntax);

    // Add path used to insert or overwrite parse/render rules.
    $pear_override = $this->pearOverridePath();
    $wiki->addPath('parse',  $pear_override . '/parse');
    $wiki->addPath('render', $pear_override . '/render');

    // General parse setup.
    if ($this->settings['ignore_regexp']) {
      $wiki->insertRule('ignoreregexp', 'delimiter');
      $wiki->setParseConf('ignoreregexp', 'ignore_regexp', $this->settings['ignore_regexp']);
    }
    if ($this->settings['ignore_tags']) {
      $wiki->insertRule('ignoretag', 'delimiter');
      $wiki->setParseConf('ignoretag', 'ignore_tags', $this->settings['ignore_tags']);
    }
    if ($this->settings['allow_html']) {
      $wiki->insertRule('ignorehtml', 'delimiter');
    }
    if (!$this->settings['use_wiki_links']) {
      $wiki->disableRule('wikilink');
    }

    // General render setup
    $wiki->setRenderConf('xhtml', 'wikilink', array(
      'view_url' => base_path(),
      'new_url' => base_path(),
      'new_text' => NULL,
      'wikilinks_plugin' => $this->settings['wikilinks_plugin'],
      'wikilinks_content_types' => array_filter($this->settings['wikilinks_content_types']),
    ));

    $wiki->setRenderConf('xhtml', 'interwiki', 'target', NULL);
    $wiki->setRenderConf('xhtml', 'interwiki', 'sites', $this->interwikiMappings());
    $wiki->setRenderConf('xhtml', 'url', 'target', NULL);
    $wiki->setRenderConf('xhtml', 'image', 'base', $this->settings['image_base']);

    // Load format dependent options.
    $this->configureWiki($wiki);

    $wiki->parse($text);

    // Note: for the format config the X of Xhtml has to be in uppercase in the CVS Version of 12/20/06
    // if this options is not set, special characters like ä,ö,ü dont work properly although the default encoding allow them
    $wiki->setFormatConf('Xhtml', 'translate', HTML_SPECIALCHARS);

    $output = $wiki->render();

    if ($this->settings['allow_html']) {
      $output = filter_xss_admin($output);
    }
    return $output;
  }
}

class PearwikiFilterBBCode extends PearwikiFilter {
  public function title() {
    return 'BBCode';
  }

  protected function syntaxUrl() {
    return 'http://en.wikipedia.org/wiki/BBCode';
  }
}

class PearwikiFilterCowiki extends PearwikiFilter {
  public function title() {
    return 'CoWiki';
  }

  protected function syntaxUrl() {
    return 'http://cowiki.org/';
  }

  public function tips($long = FALSE) {
    return t('This Wikitype is discontinued. You should not use this format.');
  }
}

class PearwikiFilterCreole extends PearwikiFilter {
  public function title() {
    return 'Creole';
  }

  protected function syntaxUrl() {
    return 'http://www.wikicreole.org/wiki/EditPageHelp';
  }
}

class PearwikiFilterDefault extends PearwikiFilter {
  public function title() {
    return 'Default';
  }

  protected function syntaxUrl() {
    return 'http://wiki.ciaweb.net/yawiki/index.php?area=Text_Wiki&page=SamplePage';
  }
}

class PearwikiFilterDoku extends PearwikiFilter {
  public function title() {
    return 'DokuWiki';
  }

  protected function syntaxUrl() {
    return 'http://wiki.splitbrain.org/wiki:syntax';
  }
}

class PearwikiFilterMediawiki extends PearwikiFilter {
  public function title() {
    return 'Mediawiki';
  }

  protected function syntaxUrl() {
    return 'http://meta.wikimedia.org/wiki/Cheatsheet';
  }

  public function tips($long = FALSE) {
    $output = '';
    $interwiki_help = $this->interwikiTips(TRUE);
    if ($long) {
      $output .= '<p>'. t('You can use <a href="!syntax_url"><em>Mediawiki</em> syntax</a>. It is possible that not all formatting options are supported at the moment.', array('!syntax_url' => $this->syntaxUrl())) .'</p>';
      $output .= '<p>'. t('You can link to internal content by using the title of a node. Use [[Node Title]] to link to the node with the corrseponding name. An alternate title can be specified like this: [[Node Title|Link title]]') .'</p>';
      $output .= '<p>'. t('External links are detected automatically. So just type http://example.com for the link. If you want a different title, use the following syntax: [http://example.com some link title].') .'</p>';
      if ($interwiki_help) {
        $output .= '<p>'. t('You can use interwiki links. Type [[site:Page Title]] to link to a page on a different site.');
        $output .= ' '. substr($interwiki_help, 3); // TODO WTF???
        $output .= '<p>'. t('Images are included with [[Image:filename]].');
        $output .= t('Special image formatting can be done via [[Image:name|alignment|size|caption]] where any of additional information is optional. <em>alignment</em> is one of <em>left</em>, <em>center</em> or <em>right</em>. Size is in the format <em>80px</em> or <em>80x70px</em>. The caption is arbitrary text. See the <a href="@url">wikipedia help</a> for the full syntax, but be aware that frames and description text is not supported.', array('@url' => 'http://en.wikipedia.org/wiki/Wikipedia:Extended_image_syntax')) .'</p>';
      }
    }
    else {
      $output .= '<p>'. t('You can use <a href="!syntax_url"><em>Mediawiki</em> syntax</a>. It is possible that not all formatting options are supported at the moment.', array('!syntax_url' => $this->syntaxUrl())) .'<br/>';
      $output .= t('Links to other pages: [[Page Title]] or [[path/to/page|Title]].') .'<br/>';
      $output .= t('External links: http://example.com or [http://example.com some link title].') .'<br/>';
      if ($interwiki_help) {
        $output .= t('Interwiki links: [[site:Page Title]].') .'<br/>';
        $output .= $interwiki_help .'<br/>';
      }
      $output .= t('Images are included with [[Image:name]]. <a href="@url">(wikipedia help)</a>', array('@url' => 'http://en.wikipedia.org/wiki/Wikipedia:Extended_image_syntax'));
      $output .= '</p>';
    }
    return $output;
  }

  protected function configureWiki(&$wiki) {
    $wiki->addPath('parse', $this->pearOverridePath() . '/parse_mediawiki');
    $wiki->insertRule('image', 'break');
    $wiki->insertRule('interwiki', 'image');
  }
}

class PearwikiFilterTiki extends PearwikiFilter {
  public function title() {
    return 'TikiWiki';
  }

  protected function syntaxUrl() {
    return 'http://tikiwiki.org/tiki-index.php?page=WikiSyntax';
  }

  public function tips($long = FALSE) {
    $output = '';
    $output .= '<p>'. t('You can use <a href="!syntax_url"><em>TikiWiki</em> syntax</a>. It is possible that not all formatting options are supported at the moment.', array('!syntax_url' => $this->syntaxUrl())) .'</p>';
    $output .= '<p>'. t('You can link to internal content by using the page title or the path. A link to a node can either be made using ((Node Title)) or ((node/3)) using the corresponding node id.') .'</p>';
    $output .= '<p>'. t('External links are detected automatically. If you want a description, use the following syntax: [http://example.com some|link title].') .'</p>';
    return $output;
  }
}
