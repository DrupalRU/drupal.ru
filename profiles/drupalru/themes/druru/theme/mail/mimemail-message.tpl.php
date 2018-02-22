<?php

/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[module]--[key].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $recipient: The recipient of the message
 * - $subject: The message subject
 * - $body: The message body
 * - $css: Internal style sheets
 * - $module: The sending module
 * - $key: The message identifier
 * - $logo: The site logo for mail
 * - $menu_top: Main menu
 * - $menu_bottom: Secondary menu
 * - $base_path: Site base url
 *
 * @var string $logo        Logo path.
 * @var string $subject     Message theme.
 * @var array  $menu_top    Main menu.
 * @var array  $menu_bottom Secondary menu.
 *
 * @see template_preprocess_mimemail_message()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="ru" xml:lang="ru" style="background:#f3f3f3!important">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <meta name="viewport" content="width=device-width">
  <title></title>
  <style>@media only screen {
      html {min-height: 100%;background: #f3f3f3}
    }
    @media only screen and (max-width: 596px) {
      .small-float-center {margin: 0 auto !important;float: none !important;text-align: center !important}
    }
    @media only screen and (max-width: 596px) {
      table.body img {width: auto;height: auto}
      table.body center {min-width: 0 !important}
      table.body .container {width: 95% !important}
      table.body .columns {height: auto !important;-moz-box-sizing: border-box;-webkit-box-sizing: border-box;box-sizing: border-box;padding-left: 16px !important;padding-right: 16px !important}
      table.body .collapse .columns {padding-left: 0 !important;padding-right: 0 !important}
      th.small-12 {display: inline-block !important;width: 100% !important}
      table.menu {width: 100% !important}
      table.menu td, table.menu th {width: auto !important;display: inline-block !important}
      table.menu[align=center] {width: auto !important}
    }</style>
</head>
<body style="-moz-box-sizing:border-box;-ms-text-size-adjust:100%;-webkit-box-sizing:border-box;-webkit-text-size-adjust:100%;Margin:0;background:#f3f3f3!important;box-sizing:border-box;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;min-width:100%;padding:0;text-align:left;width:100%!important">
<span class="preheader" style="color:#f3f3f3;display:none!important;font-size:1px;line-height:1px;max-height:0;max-width:0;mso-hide:all!important;opacity:0;overflow:hidden;visibility:hidden"></span>
<table class="wrapper body collapse dru" align="center" style="Margin:0;background:#f3f3f3!important;background-color:#3D76A0;border-collapse:collapse;border-spacing:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;height:auto!important;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;width:100%">
  <tr style="padding:0;text-align:left;vertical-align:top">
    <td class="wrapper-inner" style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">
      <table align="center" class="container" style="Margin:0 auto;background:#fefefe;border-collapse:collapse;border-spacing:0;margin:0 auto;padding:0;text-align:inherit;vertical-align:top;width:580px">
        <tbody>
        <tr style="padding:0;text-align:left;vertical-align:top">
          <td style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">
            <table class="row collapse dru" style="background-color:#3D76A0;border-collapse:collapse;border-spacing:0;display:table;padding:0;position:relative;text-align:left;vertical-align:top;width:100%">
              <tbody>
              <tr style="padding:0;text-align:left;vertical-align:top">
                <th class="small-12 large-6 columns first" style="Margin:0 auto;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0 auto;padding:0;padding-bottom:0;padding-left:0;padding-right:0;text-align:left;width:298px">
                  <table style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                    <tr style="padding:0;text-align:left;vertical-align:top">
                      <th style="Margin:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;padding:0;text-align:left">
                        <?php if ($menu_top): ?>
                          <img class="small-float-center" src="<?php print $logo; ?>" alt="Drupal.ru Logo" style="-ms-interpolation-mode:bicubic;clear:both;display:block;max-width:100%;outline:0;text-decoration:none;width:auto">
                        <?php endif; ?>
                      </th>
                    </tr>
                  </table>
                </th>
                <th class="small-12 large-6 columns last" style="Margin:0 auto;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0 auto;padding:0;padding-bottom:16px;padding-left:0;padding-right:0;text-align:left;width:298px">
                  <table style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                    <tr style="padding:0;text-align:left;vertical-align:top">
                      <th style="Margin:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;padding:0;text-align:left">
                        <?php if ($menu_top): ?>
                          <table class="spacer" style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                            <tbody>
                            <tr style="padding:0;text-align:left;vertical-align:top">
                              <td height="25px" style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:25px;font-weight:400;hyphens:auto;line-height:25px;margin:0;mso-line-height-rule:exactly;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">&#xA0;</td>
                            </tr>
                            </tbody>
                          </table>
                          <table style="border-collapse:collapse;border-spacing:0;margin-bottom:0;padding:0;text-align:left;vertical-align:top;width:100%" class="menu">
                            <tr style="padding:0;text-align:left;vertical-align:top">
                              <td style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">
                                <table style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                                  <tr style="padding:0;text-align:left;vertical-align:top">
                                    <?php foreach ($menu_top as $menu_top_item): ?>
                                      <?php if ($menu_top_item): ?>
                                        <th class="menu-item" style="Margin:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;padding:10px;padding-right:10px;text-align:left">
                                          <a href="<?php print $menu_top_item['link']; ?>" style="Margin:0;color:#fff;font-family:Helvetica,Arial,sans-serif;font-weight:400;line-height:1.3;margin:0;padding:0;text-align:left;text-decoration:none"><?php print $menu_top_item['title']; ?></a>
                                        </th>
                                      <?php endif; ?>
                                    <?php endforeach; ?>
                                  </tr>
                                </table>
                              </td>
                            </tr>
                          </table>
                        <?php endif; ?>
                      </th>
                    </tr>
                  </table>
                </th>
              </tr>
              </tbody>
            </table>
          </td>
        </tr>
        </tbody>
      </table>
    </td>
  </tr>
</table>
<table class="wrapper body" align="center" style="Margin:0;background:#f3f3f3!important;border-collapse:collapse;border-spacing:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;height:auto!important;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;width:100%">
  <tr style="padding:0;text-align:left;vertical-align:top">
    <td class="wrapper-inner" style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">
      <table align="center" class="container" style="Margin:0 auto;background:#fefefe;border-collapse:collapse;border-spacing:0;margin:0 auto;padding:0;text-align:inherit;vertical-align:top;width:580px">
        <tbody>
        <tr style="padding:0;text-align:left;vertical-align:top">
          <td style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">
            <table class="row" style="border-collapse:collapse;border-spacing:0;display:table;padding:0;position:relative;text-align:left;vertical-align:top;width:100%">
              <tbody>
              <tr style="padding:0;text-align:left;vertical-align:top">
                <th class="small-12 large-12 columns first last" style="Margin:0 auto;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0 auto;padding:0;padding-bottom:16px;padding-left:16px;padding-right:16px;text-align:left;width:564px">
                  <table style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                    <tr style="padding:0;text-align:left;vertical-align:top">
                      <th style="Margin:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;padding:0;text-align:left">
                        <table class="spacer" style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                          <tbody>
                          <tr style="padding:0;text-align:left;vertical-align:top">
                            <td height="25px" style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:25px;font-weight:400;hyphens:auto;line-height:25px;margin:0;mso-line-height-rule:exactly;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">&#xA0;</td>
                          </tr>
                          </tbody>
                        </table>
                        <h1 class="text-center" style="Margin:0;Margin-bottom:10px;color:inherit;font-family:Helvetica,Arial,sans-serif;font-size:34px;font-weight:400;line-height:1.3;margin:0;margin-bottom:10px;padding:0;text-align:center;word-wrap:normal">
                          <?php print $subject; ?>
                        </h1>
                      </th>
                      <th class="expander" style="Margin:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;padding:0!important;text-align:left;visibility:hidden;width:0"></th>
                    </tr>
                  </table>
                </th>
              </tr>
              </tbody>
            </table>
            <table class="row" style="border-collapse:collapse;border-spacing:0;display:table;padding:0;position:relative;text-align:left;vertical-align:top;width:100%">
              <tbody>
              <tr style="padding:0;text-align:left;vertical-align:top">
                <th class="small-12 large-12 columns first last" style="Margin:0 auto;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0 auto;padding:0;padding-bottom:16px;padding-left:16px;padding-right:16px;text-align:left;width:564px">
                  <table style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                    <tr style="padding:0;text-align:left;vertical-align:top">
                      <th style="Margin:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;padding:0;text-align:left">
                        <?php print $body; ?>
                        <table class="spacer" style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                          <tbody>
                          <tr style="padding:0;text-align:left;vertical-align:top">
                            <td height="15px" style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:15px;font-weight:400;hyphens:auto;line-height:15px;margin:0;mso-line-height-rule:exactly;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">&#xA0;</td>
                          </tr>
                          </tbody>
                        </table>
                      </th>
                      <th class="expander" style="Margin:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;padding:0!important;text-align:left;visibility:hidden;width:0"></th>
                    </tr>
                  </table>
                </th>
              </tr>
              </tbody>
            </table>
          </td>
        </tr>
        </tbody>
      </table>
    </td>
  </tr>
</table>
<table class="wrapper body dru" align="center" style="Margin:0;background:#f3f3f3!important;background-color:#3D76A0;border-collapse:collapse;border-spacing:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;height:auto!important;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;width:100%">
  <tr style="padding:0;text-align:left;vertical-align:top">
    <td class="wrapper-inner" style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">
      <table align="center" class="container" style="Margin:0 auto;background:#fefefe;border-collapse:collapse;border-spacing:0;margin:0 auto;padding:0;text-align:inherit;vertical-align:top;width:580px">
        <tbody>
        <tr style="padding:0;text-align:left;vertical-align:top">
          <td style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">
            <table class="row collapse dru" style="background-color:#3D76A0;border-collapse:collapse;border-spacing:0;display:table;padding:0;position:relative;text-align:left;vertical-align:top;width:100%">
              <tbody>
              <tr style="padding:0;text-align:left;vertical-align:top">
                <th class="small-12 large-6 columns first last" valign="middle" style="Margin:0 auto;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0 auto;padding:0;padding-bottom:16px;padding-left:0;padding-right:0;text-align:left;width:298px">
                  <table style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                    <tr style="padding:0;text-align:left;vertical-align:top">
                      <th style="Margin:0;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0;padding:0;text-align:left">
                        <center data-parsed="" style="min-width:242px;width:100%">
                          <?php if ($menu_bottom): ?>
                            <table class="spacer float-center" style="Margin:0 auto;border-collapse:collapse;border-spacing:0;float:none;margin:0 auto;padding:0;text-align:center;vertical-align:top;width:100%">
                              <tbody>
                              <tr style="padding:0;text-align:left;vertical-align:top">
                                <td height="16px" style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:16px;margin:0;mso-line-height-rule:exactly;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">&#xA0;</td>
                              </tr>
                              </tbody>
                            </table>
                            <table style="Margin:0 auto;border-collapse:collapse;border-spacing:0;float:none;margin:0 auto;margin-bottom:0;padding:0;text-align:center;vertical-align:top;width:auto!important" align="center" class="menu float-center">
                              <tr style="padding:0;text-align:left;vertical-align:top">
                                <td style="-moz-hyphens:auto;-webkit-hyphens:auto;Margin:0;border-collapse:collapse!important;color:#0a0a0a;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;hyphens:auto;line-height:1.3;margin:0;padding:0;text-align:left;vertical-align:top;word-wrap:break-word">
                                  <table style="border-collapse:collapse;border-spacing:0;padding:0;text-align:left;vertical-align:top;width:100%">
                                    <tr style="padding:0;text-align:left;vertical-align:top">
                                      <?php foreach ($menu_bottom as $menu_bottom_item): ?>
                                        <?php if ($menu_bottom_item): ?>
                                          <th class="menu-item float-center" style="Margin:0 auto;color:#0a0a0a;float:none;font-family:Helvetica,Arial,sans-serif;font-size:16px;font-weight:400;line-height:1.3;margin:0 auto;padding:10px;padding-right:10px;text-align:center">
                                            <a href="<?php print $menu_bottom_item['link']; ?>" style="Margin:0;color:#fff;font-family:Helvetica,Arial,sans-serif;font-weight:400;line-height:1.3;margin:0;padding:0;text-align:left;text-decoration:none"><?php print $menu_bottom_item['title']; ?></a>
                                          </th>
                                        <? endif; ?>
                                      <? endforeach; ?>
                                    </tr>
                                  </table>
                                </td>
                              </tr>
                            </table>
                          <?php endif; ?>
                        </center>
                      </th>
                    </tr>
                  </table>
                </th>
              </tr>
              </tbody>
            </table>
          </td>
        </tr>
        </tbody>
      </table>
    </td>
  </tr>
</table><!-- prevent Gmail on iOS font size manipulation -->
<div style="display:none;white-space:nowrap;font:15px courier;line-height:0">&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;</div>
</body>
</html>
