===========================================================
РУССКАЯ ВЕРСИЯ
(English version below)

Модуль позволяет полностью закрыть внешние ссылки на сайте от индексации и сохранить их валидность.

------------------
Возможности модуля
* Два метода контроля индексации: 
   * Обернуть ссылки тегом NOINDEX. Тег NOINDEX не является валидным HTML-тегом. 
     Он был создан Yandex и принят Rambler. Google игнорирует этот тег. HTML-валидаторы считают этот тег ошибкой.
   * Добавить атрибут rel="nofollow" в ссылки. Только Google не переходит по ссылкам с этим атрибутом.
* Два формата тега NOINDEX: 
   * Простой. Значение по умолчанию. HTML-валидацию не пройдет.
   * Валидный. Пройдет HTML-валидацию.
* Есть 2 списка доменов: 
   * Всегда разрешённые домены. Поисковые системы будут всегда индексировать и переходить по ссылкам на эти домены с этого сайта.
   * Всегда запрещённые домены. Поисковые системы никогда не будут индексировать и переходить по ссылкам с этого сайта на эти домены.
* Ссылки на собственный домен разрешены к индексации и переходам по умолчанию.

----------------------
Особенности применения
* Модуль реализует фильтр ввода, а значит применяется к тексту ноды и комментариев. 
  Он не влияет на блоки и ссылки в шаблоне темы - закрыть их вам придется вручную.
* Так как это фильтр ввода, то его нужно включить для каждого формата ввода, который используется на сайте.

-------------
Благодарности
Если модуль оказался вам полезен и/или помог увеличить доход, то прошу поддержать мою веру в человечество:
* WebMoney: Z348204715180
* Яндекс.Деньги: 41001200450647
* Спонсировать статью с решением, которое вам интересно.(http://drupalcookbook.ru/poisk-reshenija#sponsor)

"Разве вера в человечество не стоит 1 миллиона рублей?" (Ильф и Петров. "Золотой теленок")

===========================================================
ENGLISH VERSION
(Русская версия выше)

Module let's you take full control of external links indexing. Pages could also pass HTML-validation.

-------------
Features
* Two methods of indexing control: 
   * Wrap links with NOINDEX tag. NOINDEX tag is used to deny indexing part of page. 
     Only Yandex and Rambler considers this tag but Google and others doesn't. There is also an attribute NOINDEX in meta-tag Robots.
   * Add attribute rel="nofollow" to links. This link's attribute has no effect on indexing. 
     Only Google really do not follow this links but others do. 
     Attribute is used to say to search engine that links no longer constitute a "vote" in the PageRank system.
* Two NOINDEX-tags formats: 
   * Simple. This is a default. HTML-validators considers this tag as mistake so your pages will not pass HTML-validation.
   * Valid. Pages will pass HTML-validation.
* Two domain lists: 
   * Always allowed domains. Search Engines will always index and follow links to this domains at this site.
   * Always denied domains. Search Engines will never index and follow links to this domains at this site.
* Links to your own domain is allowed by default for indexing and following.

-------------
Tips and tricks
* This module defines an input filter and can be applied to nodes and comments body. 
  Links in blocks, menus and theme files should be fixed by hand.
* This input filter should be enabled for each input format used at site.
* CCK fields (Links) isn't covered too. Solution: get object $node in node.tpl.php, print CCK-fields using custom code.


===========================================================
Контакты (Contacts)

Vlad Savitsky
ICQ: 205535814
http://vladsavitsky.ru/contacts
