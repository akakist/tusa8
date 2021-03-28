

-- MySQL dump 10.9
--
-- Host: localhost    Database: chat
-- ------------------------------------------------------
-- Server version	4.1.21

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `credits_transactions`
--

DROP TABLE IF EXISTS `credits_loans`;
CREATE TABLE `credits_loans` (
  `id` int(20) unsigned NOT NULL auto_increment,
  `summa` double NOT NULL default '0',
  `from_user` int(20) unsigned NOT NULL default '0',
  `to_user` int(20) unsigned NOT NULL default '0',
  `to_nick` char(16) NOT NULL default '',
  `committed` char(1) NOT NULL default '0',
  `pogashen` char(1) NOT NULL default '0',
  `create_time` datetime default NULL,
  `end_time` datetime default NULL,
  `commit_time` datetime default NULL,
  PRIMARY KEY  (`id`),
  KEY `_fu` (`from_user`),
  KEY `_tu` (`to_user`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;

#select pogashen from credits_loans where (committed=0 or pogashen=0) and (from_user=26 or to_user=26) Error:Table 'chat.credits_loans' doesn't exist---THREAD REPORT -1448129648 (0xA9AF4B90)


--
-- Dumping data for table `credits_transactions`
--

LOCK TABLES `credits_transactions` WRITE;
/*!40000 ALTER TABLE `credits_transactions` DISABLE KEYS */;
/*!40000 ALTER TABLE `credits_transactions` ENABLE KEYS */;
UNLOCK TABLES;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

alter table user_profiles add column family_status text not null default '';
alter table user_profiles add column hide_family_status char(1) not null default '0';
