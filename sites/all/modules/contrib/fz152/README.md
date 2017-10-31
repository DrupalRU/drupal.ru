## FZ152

This module helps you to bypass federal law 152 in Russian Federation.

He provides simple tools and API to adding privacy policy page on the site and adds checkbox 'I agree to process my personal data' to any form you want it.
 
**For what this module created**:

- [Федеральный закон 152](http://www.consultant.ru/document/cons_doc_LAW_61801/) — reason for this module.
- [Всем, у кого есть сайт! С 1 июля штрафы за нарушение закона о персональных данных увеличатся до 75 тысяч рублей](https://journal.tinkoff.ru/personalnye-dannye/)

**Requirements**

- [variable](https://www.drupal.org/project/variable) module

**Installation**

Just download and install as other Drupal modules. Then navigate to module list page and enable it.

**Features**

- Add's checkbox to any form you wants with confirmation about personal information.
- Everything is customizible and translatable.
- You can disable this functionality for different languages, or eve set up this module to work different per language.
- Simple UI to handle all form on the site. Don't require any coding, you can do almost everything in UI.
- Support for wildcard in form id's.
- You can handle weight of checkbox in the forms (except webforms, due their architecture).
- This module adds `/privacy-policy` page by default with unified legal agreement, that can fit any site with any purposes. Big thanks [RaDon company](http://www.ra-don.ru/) for providing this legal agreement!
- You can edit this text via UI with prefered text format.
- You can edit path for this page, even set different paths for different languages.
- You can disable this page and use your own.
- Integration with [Entityform](https://www.drupal.org/project/entityform) module. (submodule, require enabling `fz152_entityform`)
- Integration with [Webform](https://www.drupal.org/project/webform) module. (submodule, require enabling `fz152_webform`)
- Provide several hooks, see **fz152.api.php**. They helps you to extend module or modify something if you want!

**Screenshots**

![Main settings](http://i.imgur.com/AZ7UnYB.png)

![Privacy policy page](http://i.imgur.com/utnHi5T.png)

![Manual form settings](http://i.imgur.com/MNG9kMe.png)

![Entityform settings](http://i.imgur.com/kMSlU4h.png)