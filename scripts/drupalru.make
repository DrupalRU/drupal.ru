; Drupal.ru drush make file for dev env
; REQUIRED ATTRIBUTES
api = 2
core = 7.x

; Stable versions
projects[] = drupal

; Contrib modules
; Libraries module goes first
projects[libraries][subdir] = "contrib"

projects[ban_user][subdir] = "contrib"
projects[bbcode][subdir] = "contrib"
projects[bueditor][subdir] = "contrib"
projects[captcha][subdir] = "contrib"
projects[captcha_pack][subdir] = "contrib"
projects[comment_notify][subdir] = "contrib"
projects[ctools][subdir] = "contrib"
projects[date][subdir] = "contrib"
projects[fontawesome][subdir] = "contrib"
projects[geshifilter][subdir] = "contrib"
projects[gravatar][subdir] = "contrib"
projects[imageapi][subdir] = "contrib"
projects[jquery_update][subdir] = "contrib"
projects[l10n_update][subdir] = "contrib"
projects[metatag][subdir] = "contrib"
projects[noindex_external_links][subdir] = "contrib"
projects[pathauto][subdir] = "contrib"
projects[privatemsg][subdir] = "contrib"
projects[rrssb][subdir] = "contrib"
projects[simplenews][subdir] = "contrib"
projects[smtp][subdir] = "contrib"
projects[spambot][subdir] = "contrib"
projects[token][subdir] = "contrib"
projects[transliteration][subdir] = "contrib"
projects[xbbcode][subdir] = "contrib"

; Contrib dev modules
projects[quote][version] = "1.x-dev"
projects[quote][subdir] = "contrib"

projects[diff][version] = "3.x-dev"
projects[diff][subdir] = "contrib"

; Contrib theme
projects[bootstrap_lite][subdir] = "contrib"

; git versions
projects[altpager][type] = "module"
projects[altpager][download][type] = "git"
projects[altpager][download][url] = "https://github.com/itpatrol/altpager"
projects[altpager][download][branch] = "master"
projects[altpager][subdir] = "github"

projects[alttracker][type] = "module"
projects[alttracker][download][type] = "git"
projects[alttracker][download][url] = "https://github.com/itpatrol/alttracker"
projects[alttracker][download][branch] = "master"
projects[alttracker][subdir] = "github"

projects[drupal_deploy][type] = "module"
projects[drupal_deploy][download][type] = "git"
projects[drupal_deploy][download][url] = "https://github.com/itpatrol/drupal_deploy"
projects[drupal_deploy][download][branch] = "7.x"
projects[drupal_deploy][subdir] = "github"

projects[inner_poll][type] = "module"
projects[inner_poll][download][type] = "git"
projects[inner_poll][download][url] = "http://git.drupal.org/sandbox/andypost/1413472.git"
projects[inner_poll][download][branch] = "7.x-1.x"
projects[inner_poll][subdir] = "github"


; Libraries
libraries[fontawesome][download][type] = "git"
libraries[fontawesome][download][url] = "https://github.com/FortAwesome/Font-Awesome.git"

libraries[rrssb][download][type] = "file"
libraries[rrssb][download][url] = "https://github.com/kni-labs/rrssb/archive/1.8.1.zip"
libraries[rrssb][directory_name] = "rrssb"
libraries[rrssb][type] = "library"

libraries[geshi][type] = "library"
libraries[geshi][download][type] = "file"
libraries[geshi][download][url] = "http://sourceforge.net/projects/geshi/files/geshi/GeSHi%201.0.8.10/GeSHi-1.0.8.10.tar.gz/download"

; not really libraries... but I don't see other way
libraries[drupalru][type] = "library"
libraries[drupalru][download][type] = "git"
libraries[drupalru][download][url] = "http://github.com/drupalru/drupal.ru"
libraries[drupalru][download][branch] = "stage"
libraries[drupalru][destination] = "sites/default/"

; Load Russian translations.
; translations[] = ru

//advanced_sphinx, antinoob_validate, antiswearing_validate, darkmatter, dru_claim, dru_comment_quote, dru_forum, dru_frontpage, dru_tickets, dru_tnx, marketplace, resolve, simple_events, user_filter,user_filter_notify, validate_api, xbbcode_dru, alpha