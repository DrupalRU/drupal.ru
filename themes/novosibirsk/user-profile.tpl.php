<?php
// $Id: user-profile.tpl.php,v 1.2 2007/08/07 08:39:36 goba Exp $

/**
 * @file user-profile.tpl.php
 * Default theme implementation to present all user profile data.
 *
 * This template is used when viewing a registered member's profile page,
 * e.g., example.com/user/123. 123 being the users ID.
 *
 * By default, all user profile data is printed out with the $user_profile
 * variable. If there is a need to break it up you can use $profile instead.
 * It is keyed to the name of each category or other data attached to the
 * account. If it is a category it will contain all the profile items. By
 * default $profile['summary'] is provided which contains data on the user's
 * history. Other data can be included by modules. $profile['picture'] is
 * available by default showing the account picture.
 *
 * Also keep in mind that profile items and their categories can be defined by
 * site administrators. They are also available within $profile. For example,
 * if a site is configured with a category of "contact" with
 * fields for of addresses, phone numbers and other related info, then doing a
 * straight print of $profile['contact'] will output everything in the
 * category. This is useful for altering source order and adding custom
 * markup for the group.
 *
 * To check for all available data within $profile, use the code below.
 *
 *   <?php print '<pre>'. check_plain(print_r($profile, 1)) .'</pre>'; ?>
 *
 * @see user-profile-category.tpl.php
 *      where the html is handled for the group.
 * @see user-profile-field.tpl.php
 *      where the html is handled for each item in the group.
 *
 * Available variables:
 * - $user_profile: All user profile data. Ready for print.
 * - $profile: Keyed array of profile categories and their items or other data
 *   provided by modules.
 *
 * @see template_preprocess_user_profile()
 */
 //global $user;
 //if($user->uid == 2164) foreach($profile as $k => $v)drupal_set_message($k . " = " . $v); 
 //if($user->uid == 2164) drupal_set_message("12: " . $profile["uid"]);
 $dru_assoc = "";
	if(arg(0) == "user" && is_numeric(arg(1))) {
		$uid = arg(1);
		$r = db_fetch_object(db_query("SELECT v.value FROM {profile_values} v INNER JOIN {profile_fields} f ON f.fid = v.fid WHERE v.uid = %d AND f.name = 'profile_drupal_association_id'", $uid));
		if($r->value != "")$dru_assoc = "<div class=\"drupal_assoc\"><a href=\"http://association.drupal.org/user/" . $r->value . "\"><img src=\"" . base_path() . path_to_theme() . "/img/DA-individual-80.png\"></a></div>";
		
		$acc = user_load($uid);
	}
    $uid = arg(1);
    $r = db_fetch_object(db_query("SELECT v.value FROM {profile_values} v INNER JOIN {profile_fields} f ON f.fid = v.fid WHERE v.uid = %d AND f.name = 'profile_drupal_association_id'", $uid));
    if($r->value != "")$dru_assoc = "<div class=\"drupal_assoc\"><a href=\"http://association.drupal.org/user/" . $r->value . "\"><img src=\"" . base_path() . path_to_theme() . "/img/DA-individual-80.png\"></a></div>";
    
    $acc = user_load($uid);
  }
 
?>

<div class="profile">   
  <h2>Профиль пользователя <?php print $acc->name; ?></h2><br>
    <div class="column">
      <h2>Работа</h2>
      <?php
        if($profile["Предлагаю_сервисы_для_Drupal"] != "")print  "<br><h4>Предлагаю услуги</h4><br>" . str_replace("<h3>Предлагаю_сервисы_для_Drupal</h3>", "", $profile["Предлагаю_сервисы_для_Drupal"]);
        if($profile["Мои_работы_для_Drupal"] != "")print  "<br><h4>Мои работы для сообщества Drupal</h4><br>" . str_replace("<h3>Мои_работы_для_Drupal</h3>", "", $profile["Мои_работы_для_Drupal"]);        
      ?>
      <h2>История</h2>
      <?php
        if($profile["summary"] != "")print  "<br>" . str_replace("<h3>История</h3>", "", $profile["summary"]);
        print '<br>' . 'Заходил: ' .  format_interval(time() - $acc->access) . ' назад';
        $nodes = db_result(db_query("SELECT COUNT(0) FROM {node} WHERE uid = %d", $acc->uid));
        $comments = db_result(db_query("SELECT COUNT(0) FROM {comments} WHERE uid = %d", $acc->uid));
        $pm_count = db_result(db_query("SELECT COUNT(0) FROM {pm_index} WHERE deleted = 0 AND  uid = %d", $acc->uid));
        print '<br>Материалов: ' . $nodes . ', комментариев: ' . $comments . ' (pm '. $pm_count . ')';
        // Remove link to user homepage.
        if ($uid == 3016) { // andypost, for tests
          //print_r($profile);
        }
      ?>
    </div>
    <div class="column">
      <h2>Личные данные</h2>
      <?php
        if($profile["user_picture"] != "" || $dru_assoc != "") print  "<div class=\"right_top\">";
        if($profile["user_picture"] != "") print $profile["user_picture"];
        if($dru_assoc != "") print $dru_assoc;
        if($profile["user_picture"] != "" || $dru_assoc != "") print  "</div>"; 
        if($profile["Персональные_данные"] != "")print  "<br>" . str_replace("<h3>Персональные_данные</h3>", "", $profile["Персональные_данные"]);
        if(($profile["Координаты_в_интернете"] != "") AND (($nodes > 0) OR ($comments > 10)) ) print  "<br>" . str_replace("<h3>Координаты_в_интернете</h3>", "", $profile["Координаты_в_интернете"]);
        if($profile["privatemsg_send_new_message"] != "")print  str_replace("Send Message", "Отправить личное сообщение", $profile["privatemsg_send_new_message"]);        

      ?>
    </div>
    <div class="column_footer">
    </div>
<?php    
    //print $user_profile; 
 ?>
</div>
