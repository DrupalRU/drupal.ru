# Модуль Validate API

Отдельное спасибо @Gormartsen за участие в разработке!

Позволяет использовать собственные валидаторы для полей материалов и комментариев.

Включить существующие валидаторы можно на странице настройки материала, во вкладке "API проверки содержимого". 
Доступные валидаторы для комментариев находятся во вкладке настройки комментариев.

### Пример добавления валидатора: 

```php
/**
 * Implements hook_validator_info().
 */
function MYMODULE_validator_info(){
  
  $items = array();
  
  $items[] = array(
    'type'        => 'text',                        // Field module
    'name'        => t('simple_validate'),          // Validator machine name
    'title'       => 'Validate API',                // Validator title
    'description' => t('Simple Discription'),       // Validator description
    'callback'    => 'simple_validator_callback',   // Validation callback
  );
  
  return $items;
  
}
```

### Пример валидации (реализация коллбека): 

```php
function simple_validator_callback($field, $field_name, $entity, &$message){
  $values = array();
  $valid = array();
  if($field_name == 'title'){
    $values[] = $field;
    $label = t('Title');
  }
  else{
    $info = field_info_instance('node', $field_name, $entity->type);
    $label = $info['label'];
    if(!empty($field['und'])){
      foreach($field['und'] as $field_value){
        $values[] = $field_value['value'];
      }
    }
  }
  
  foreach($values as $value){
    if(preg_match('/срочно/ui', $value)){
      $valid[] = FALSE;
    }
  }
  
  if(in_array(FALSE, $valid)){
    $message = t('Field "@field" isn\'t valid!', array('@field' => $label));
    return FALSE;
  }
  else{
    return TRUE;
  }
}
```

Callback возвращает булево значение "TRUE", если поле проходит валидацию, и "FALSE", если нет. Так же валидатор передает сообщение для пользователя.

**Значения аргументов:**

- **$field** - валидируемое поле (может быть значением, если передается *Title*, или массивом, если обычное поле);
- **$field_name** - машинное имя поля;
- **$entity** - валидируемая сущность (используется для получения лейбла поля);
- **$message** - сообщение выводимое пользователю, в случае не прохождения валидации.

________________

# Модуль Antinoob Validate


Добавляет несколько простых валидаторов: 

- **Exclamation point** - валидатор восклицательного знака. Запрещает публикацию, если в строке используется более 2-х восклицательных знаков подряд.
- **Caps Lock** - валидатор заглавных букв. Запрещает публикацию, если в строке используются только заглавные буквы.
- **Disallowed Words** - валидатор запрещенных слов. Запрещает публикацию, если в строке присутствуют слова из списка запрещенных.

________________

# Модуль Antiswearing Validate

Добавляет валидатор матов. Для валидации используется PHP-класс "[php-obscene-censor-rus](https://github.com/vearutop/php-obscene-censor-rus)".

- **Мат-фильтр** - валидатор матершинных слов. Запрещает публикацию, если в строке присутствует маты.
