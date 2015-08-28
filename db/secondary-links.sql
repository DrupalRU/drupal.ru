-- MySQL dump 10.13  Distrib 5.5.35, for Linux (x86_64)
--
-- Host: localhost    Database: drupal_main
-- ------------------------------------------------------
-- Server version	5.5.35-33.0

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `menu_links`
--
-- WHERE:  menu_name='secondary-links'

LOCK TABLES `menu_links` WRITE;
/*!40000 ALTER TABLE `menu_links` DISABLE KEYS */;
INSERT INTO `menu_links` VALUES ('secondary-links',122,0,'node/60','node/%','О Drupal','a:0:{}','menu',0,0,0,0,-50,1,1,122,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',123,0,'http://demo.drupal.ru','','демо-сайт','a:0:{}','menu',1,1,0,0,-47,1,1,123,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',124,0,'node/1','node/%','О Проекте','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:0:\"\";}}','menu',0,0,0,0,-42,1,1,124,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',125,0,'sites','','русские сайты','a:0:{}','menu',1,0,0,0,0,1,1,125,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',128,0,'node/31','node/%',' .RU','a:0:{}','menu',1,0,0,0,-48,1,1,128,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',180,0,'contact','contact','Контакты','a:0:{}','menu',1,0,0,0,-43,1,1,180,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('secondary-links',204,0,'whois','','whois','a:0:{}','menu',1,0,0,0,8,1,1,204,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',213,0,'site_user_list','site_user_list','Сообщество','a:0:{}','system',1,0,0,0,-6,1,1,213,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('secondary-links',218,0,'node/25','node/%','Команда','a:0:{}','menu',1,0,0,0,-44,1,1,218,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',244,0,'book','book','Документация','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:0:\"\";}}','menu',0,0,0,0,-49,1,1,244,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('secondary-links',249,0,'http://mini.drupal.ru','','КПК-версия','a:0:{}','menu',1,1,0,0,-45,1,1,249,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',251,0,'node/19222','node/%','Встречи','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:0:\"\";}}','menu',1,0,0,0,-46,1,1,251,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('secondary-links',2245,0,'node/2','node/%','Правила','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:0:\"\";}}','menu',0,0,0,0,-41,1,1,2245,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('secondary-links',2276,0,'http://moscow.drupalcamp.ru','','DrupalCamp Москва','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:51:\"Россия, Москва, 16-17 апреля 2010\";}}','menu',1,1,0,0,-40,1,1,2276,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('secondary-links',2304,0,'http://camp10.drupal.ua/','','DrupalCamp Киев','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:45:\"Украина, Киев, 10-12 июня 2010\";}}','menu',1,1,0,0,0,1,1,2304,0,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `menu_links` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-08-28  3:55:34
