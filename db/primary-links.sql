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
-- WHERE:  menu_name='primary-links'

LOCK TABLES `menu_links` WRITE;
/*!40000 ALTER TABLE `menu_links` DISABLE KEYS */;
INSERT INTO `menu_links` VALUES ('primary-links',70,0,'tracker','tracker','Новое на сайте','a:0:{}','system',0,0,0,0,-50,1,1,70,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('primary-links',113,0,'forum','forum','Форум','a:0:{}','menu',0,0,0,0,-49,1,1,113,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('primary-links',114,0,'blog','blog','Блоги','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:0:\"\";}}','menu',0,0,0,0,-46,1,1,114,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('primary-links',115,0,'weblink','','ссылки','a:0:{}','menu',1,0,0,0,0,1,1,115,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('primary-links',116,0,'http://api.drupal.ru','','API','a:0:{}','menu',0,1,0,0,-47,1,1,116,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('primary-links',117,0,'sitemap','','карта сайта','a:0:{}','menu',1,0,0,0,10,1,1,117,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('primary-links',118,0,'node/5022','node/%','Документация','a:0:{}','menu',1,0,0,0,-45,1,1,118,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('primary-links',227,0,'node/7741','node/%','Скачать','a:0:{}','menu',1,0,0,0,-45,1,1,227,0,0,0,0,0,0,0,0,1);
INSERT INTO `menu_links` VALUES ('primary-links',2259,0,'http://groups.drupal.ru','','Группы','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:39:\"Дискуссионные группы\";}}','menu',1,1,0,0,-48,1,1,2259,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('primary-links',2439,0,'aggregator/categories/1','aggregator/categories/%','Планета','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:0:\"\";}}','menu',0,0,0,0,30,1,1,2439,0,0,0,0,0,0,0,0,0);
INSERT INTO `menu_links` VALUES ('primary-links',2548,0,'http://podcasts.drupal.ru','','Подкасты','a:1:{s:10:\"attributes\";a:1:{s:5:\"title\";s:0:\"\";}}','menu',0,1,0,0,-5,1,1,2548,0,0,0,0,0,0,0,0,0);
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

-- Dump completed on 2015-08-28  3:55:54
