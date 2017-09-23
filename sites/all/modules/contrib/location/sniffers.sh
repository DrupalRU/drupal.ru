#!/bin/sh
# for using this script
# gem install css_lint
# gem install scss-lint
# pear install PHP_CodeSniffer
sudo composer global require drupal/coder:\>7
find ./ -name "*.css" -print0 | xargs -0 css_lint --quiet > csslint.txt || true
find ./ -name "*.scss" -print0 | xargs -0 scss-lint > scsslint.txt || true
sudo find ./ -type f \( -iname "*.js" ! -iname "*.min.js" \) -print0 | sudo xargs -0 jshint > jshintcontrib.txt
sudo find ./ -type f \( -iname "*.js" ! -iname "*.min.js" ! -iname "*min.js" \) -print0 | sudo xargs -0 jshint > jshintthemes.txt
sudo phpcs --standard=DrupalPractice --extensions=php,module,inc,install,test,profile,theme -n ./ --report-file=sniff2.txt
sudo phpcs --standard=/root/.composer/vendor/drupal/coder/coder_sniffer/Drupal/ --extensions=php,module,inc,install,test,profile,theme -n ./ --report-file=sniff.txt
