
DROP TABLE IF EXISTS `leaders`;
CREATE TABLE `leaders` (
  `channel` int(11) NOT NULL default '0',
  `uid` int(11) NOT NULL default '0',
  `disabled` char(1) NOT NULL default '0',
  `set_time` datetime NOT NULL default '0000-00-00 00:00:00',
  KEY `__C` (`channel`),
  KEY `__D` (`disabled`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;

