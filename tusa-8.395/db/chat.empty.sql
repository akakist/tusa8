-- MySQL dump 9.11
--
-- Host: localhost    Database: 
-- ------------------------------------------------------
-- Server version	4.0.20

--
-- Current Database: bot
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ bot;

USE bot;

--
-- Table structure for table `greetingmsg`
--

CREATE TABLE greetingmsg (
  ID int(10) unsigned NOT NULL auto_increment,
  answer text NOT NULL,
  PRIMARY KEY  (ID)
) TYPE=ISAM PACK_KEYS=1;

--
-- Dumping data for table `greetingmsg`
--


--
-- Table structure for table `msg`
--

CREATE TABLE msg (
  ID int(10) unsigned NOT NULL auto_increment,
  answer text NOT NULL,
  pattern varchar(100) NOT NULL default '',
  PRIMARY KEY  (ID),
  KEY _patter (pattern)
) TYPE=MyISAM;

--
-- Dumping data for table `msg`
--


--
-- Table structure for table `privmsg`
--

CREATE TABLE privmsg (
  ID int(10) unsigned NOT NULL auto_increment,
  answer text NOT NULL,
  pattern varchar(100) NOT NULL default '',
  PRIMARY KEY  (ID),
  KEY _patter (pattern)
) TYPE=ISAM PACK_KEYS=1;

--
-- Dumping data for table `privmsg`
--


--
-- Table structure for table `simplemsg`
--

CREATE TABLE simplemsg (
  ID int(10) unsigned NOT NULL auto_increment,
  answer text NOT NULL,
  PRIMARY KEY  (ID)
) TYPE=ISAM PACK_KEYS=1;

--
-- Dumping data for table `simplemsg`
--


--
-- Table structure for table `simpleprivmsg`
--

CREATE TABLE simpleprivmsg (
  ID int(10) unsigned NOT NULL auto_increment,
  answer text NOT NULL,
  PRIMARY KEY  (ID)
) TYPE=ISAM PACK_KEYS=1;

--
-- Dumping data for table `simpleprivmsg`
--


--
-- Table structure for table `unknow`
--

CREATE TABLE unknow (
  ID int(10) unsigned NOT NULL auto_increment,
  msg text NOT NULL,
  PRIMARY KEY  (ID)
) TYPE=ISAM PACK_KEYS=1;

--
-- Dumping data for table `unknow`
--


--
-- Current Database: chat
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ chat;

USE chat;

--
-- Table structure for table `anekdots`
--

CREATE TABLE anekdots (
  ID int(20) unsigned NOT NULL auto_increment,
  txt text,
  rating int(11) NOT NULL default '0',
  add_date datetime default NULL,
  add_by text,
  chk int(1) NOT NULL default '0',
  ip text,
  PRIMARY KEY  (ID),
  KEY chk_k (chk)
) TYPE=MyISAM;

--
-- Dumping data for table `anekdots`
--


--
-- Table structure for table `banned_ip`
--

CREATE TABLE banned_ip (
  ID int(20) unsigned NOT NULL auto_increment,
  ip text,
  proxy text,
  time_ban datetime default NULL,
  time_free datetime default NULL,
  time_cnt text,
  uid int(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY uidk (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `banned_ip`
--


--
-- Table structure for table `banned_login`
--

CREATE TABLE banned_login (
  ID int(20) unsigned NOT NULL auto_increment,
  login text,
  time_ban datetime default NULL,
  time_free datetime default NULL,
  time_cnt text,
  uid int(20) unsigned NOT NULL default '0',
  descr text NOT NULL,
  PRIMARY KEY  (ID),
  KEY uidk (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `banned_login`
--


--
-- Table structure for table `channels`
--

CREATE TABLE channels (
  ID int(20) unsigned NOT NULL auto_increment,
  censored int(2) default '1',
  save char(1) default '1',
  move_to_channel int(10) default '0',
  move char(1) default '0',
  no_stat char(1) NOT NULL default '0',
  ext char(1) NOT NULL default '0',
  read_level int(4) NOT NULL default '0',
  write_level int(4) NOT NULL default '0',
  irc_name text,
  PRIMARY KEY  (ID)
) TYPE=MyISAM;

--
-- Dumping data for table `channels`
--


--
-- Table structure for table `channels_names`
--

CREATE TABLE channels_names (
  ID int(20) unsigned NOT NULL auto_increment,
  cid int(20) unsigned NOT NULL default '0',
  lang char(3) default '',
  name text,
  PRIMARY KEY  (ID),
  KEY cid_key (cid)
) TYPE=MyISAM;

--
-- Dumping data for table `channels_names`
--


--
-- Table structure for table `credit_log`
--

CREATE TABLE credit_log (
  dt datetime default NULL,
  from_uid int(11) NOT NULL default '0',
  to_uid int(11) NOT NULL default '0',
  summa double NOT NULL default '0',
  opcode varchar(40) NOT NULL default '',
  id int(11) NOT NULL auto_increment,
  PRIMARY KEY  (id),
  KEY _DT (dt),
  KEY __F (from_uid),
  KEY __T (to_uid),
  KEY __OP (opcode)
) TYPE=MyISAM;

--
-- Dumping data for table `credit_log`
--


--
-- Table structure for table `credits_transactions`
--

CREATE TABLE credits_transactions (
  id int(20) unsigned NOT NULL auto_increment,
  summa double NOT NULL default '0',
  from_user int(20) unsigned NOT NULL default '0',
  to_user int(20) unsigned NOT NULL default '0',
  to_nick char(16) NOT NULL default '',
  committed char(1) NOT NULL default '0',
  create_time datetime default NULL,
  commit_time datetime default NULL,
  PRIMARY KEY  (id),
  KEY _fu (from_user),
  KEY _tu (to_user)
) TYPE=MyISAM;

--
-- Dumping data for table `credits_transactions`
--


--
-- Table structure for table `filters_in`
--

CREATE TABLE filters_in (
  folder_id int(11) NOT NULL default '0',
  uid int(11) NOT NULL default '0',
  KEY _kZ (folder_id)
) TYPE=MyISAM;

--
-- Dumping data for table `filters_in`
--


--
-- Table structure for table `filters_out`
--

CREATE TABLE filters_out (
  folder_id int(11) NOT NULL default '0',
  uid int(11) NOT NULL default '0',
  KEY _kZ (folder_id)
) TYPE=MyISAM;

--
-- Dumping data for table `filters_out`
--


--
-- Table structure for table `folder_contents`
--

CREATE TABLE folder_contents (
  folder_id int(11) NOT NULL default '0',
  message_id int(11) NOT NULL default '0',
  KEY _k1 (folder_id),
  KEY _k12 (message_id)
) TYPE=MyISAM;

--
-- Dumping data for table `folder_contents`
--


--
-- Table structure for table `forum_messages`
--

CREATE TABLE forum_messages (
  ID int(20) unsigned NOT NULL auto_increment,
  author varchar(20) default NULL,
  subj text,
  msg text,
  tid int(20) unsigned NOT NULL default '0',
  nid int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  ip varchar(20) default NULL,
  add_date datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (ID),
  KEY uidk (nid),
  KEY nidk (uid),
  KEY tidk (tid),
  KEY add_datek (add_date)
) TYPE=MyISAM;

--
-- Dumping data for table `forum_messages`
--


--
-- Table structure for table `forum_themes`
--

CREATE TABLE forum_themes (
  ID int(20) unsigned NOT NULL auto_increment,
  author varchar(20) default NULL,
  name text,
  nid int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  ip varchar(20) default NULL,
  add_date datetime NOT NULL default '0000-00-00 00:00:00',
  create_date datetime NOT NULL default '0000-00-00 00:00:00',
  msgcnt int(20) NOT NULL default '0',
  last_by varchar(17) NOT NULL default '',
  last_by_id int(20) unsigned default '0',
  PRIMARY KEY  (ID),
  KEY uidk (nid),
  KEY nidk (uid),
  KEY add_datek (add_date)
) TYPE=MyISAM;

--
-- Dumping data for table `forum_themes`
--


--
-- Table structure for table `funnypics`
--

CREATE TABLE funnypics (
  ID int(20) unsigned NOT NULL auto_increment,
  name text,
  descr text,
  showed int(11) NOT NULL default '0',
  rating int(11) NOT NULL default '0',
  PRIMARY KEY  (ID)
) TYPE=MyISAM;

--
-- Dumping data for table `funnypics`
--


--
-- Table structure for table `grant_agrees`
--

CREATE TABLE grant_agrees (
  ID int(20) unsigned NOT NULL auto_increment,
  gid int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  yes int(20) NOT NULL default '0',
  no int(20) NOT NULL default '0',
  login char(20) default NULL,
  PRIMARY KEY  (ID),
  KEY gidk (gid),
  KEY uidk (uid),
  KEY yesk (yes),
  KEY nok (no)
) TYPE=MyISAM;

--
-- Dumping data for table `grant_agrees`
--


--
-- Table structure for table `grant_messages`
--

CREATE TABLE grant_messages (
  ID int(20) unsigned NOT NULL auto_increment,
  gid int(20) unsigned NOT NULL default '0',
  from_nick text,
  ip varchar(20) default NULL,
  msg text,
  add_date datetime default NULL,
  show_nick char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY gidk (gid)
) TYPE=MyISAM;

--
-- Dumping data for table `grant_messages`
--


--
-- Table structure for table `grant_readden`
--

CREATE TABLE grant_readden (
  readden char(1) NOT NULL default '',
  uid int(20) unsigned NOT NULL default '0',
  gid int(20) unsigned NOT NULL default '0',
  KEY uid_key (uid),
  KEY gid_key (gid)
) TYPE=MyISAM;

--
-- Dumping data for table `grant_readden`
--


--
-- Table structure for table `grant_vote`
--

CREATE TABLE grant_vote (
  ID int(20) unsigned NOT NULL auto_increment,
  nick char(20) default NULL,
  llevel int(10) default NULL,
  nlevel int(10) default NULL,
  uid int(20) NOT NULL default '0',
  add_date datetime default NULL,
  add_by int(20) default NULL,
  status char(1) default NULL,
  PRIMARY KEY  (ID),
  KEY uidk (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `grant_vote`
--


--
-- Table structure for table `hints`
--

CREATE TABLE hints (
  ID int(20) unsigned NOT NULL auto_increment,
  txt text NOT NULL,
  lang char(3) NOT NULL default '',
  uid int(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY lang_key (lang),
  KEY uid_key (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `hints`
--


--
-- Table structure for table `histories`
--

CREATE TABLE histories (
  ID int(20) unsigned NOT NULL auto_increment,
  txt text,
  rating int(11) NOT NULL default '0',
  add_date datetime default NULL,
  add_by text,
  chk int(1) NOT NULL default '0',
  ip text,
  PRIMARY KEY  (ID),
  KEY chk_k (chk)
) TYPE=MyISAM;

--
-- Dumping data for table `histories`
--


--
-- Table structure for table `news`
--

CREATE TABLE news (
  ID int(20) unsigned NOT NULL auto_increment,
  uid int(20) unsigned NOT NULL default '0',
  add_date datetime NOT NULL default '0000-00-00 00:00:00',
  lang text,
  txt text,
  add_nid int(20) unsigned default '0',
  PRIMARY KEY  (ID),
  KEY uid_key (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `news`
--


--
-- Table structure for table `nick_vecs`
--

CREATE TABLE nick_vecs (
  refid int(20) unsigned NOT NULL default '0',
  u_nicks blob,
  PRIMARY KEY  (refid)
) TYPE=MyISAM;

--
-- Dumping data for table `nick_vecs`
--


--
-- Table structure for table `nicks`
--

CREATE TABLE nicks (
  ID int(15) unsigned NOT NULL auto_increment,
  Login varchar(18) NOT NULL default '0',
  Nick varchar(17) NOT NULL default '',
  UpperNick varchar(17) NOT NULL default '',
  Banned int(2) default NULL,
  uid int(10) NOT NULL default '0',
  bby int(10) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY _nick (Nick),
  KEY uidk (uid),
  KEY logink (Login),
  KEY bby_key (bby),
  KEY uppernick_key (UpperNick),
  KEY banned_key (Banned)
) TYPE=MyISAM;

--
-- Dumping data for table `nicks`
--


--
-- Table structure for table `note_folders`
--

CREATE TABLE note_folders (
  id int(10) unsigned NOT NULL auto_increment,
  uid int(11) NOT NULL default '0',
  name blob,
  PRIMARY KEY  (id),
  KEY _uid (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `note_folders`
--


--
-- Table structure for table `note_texts`
--

CREATE TABLE note_texts (
  message_id int(11) NOT NULL default '0',
  body blob,
  KEY _kM1 (message_id)
) TYPE=MyISAM;

--
-- Dumping data for table `note_texts`
--


--
-- Table structure for table `notes`
--

CREATE TABLE notes (
  id int(10) unsigned NOT NULL auto_increment,
  from_nick_id int(11) NOT NULL default '0',
  to_nick_id int(11) NOT NULL default '0',
  from_uid int(11) NOT NULL default '0',
  to_uid int(11) NOT NULL default '0',
  from_nick varchar(100) NOT NULL default '',
  to_nick varchar(100) NOT NULL default '',
  refcount int(11) NOT NULL default '0',
  subject blob,
  send_date datetime default NULL,
  status char(1) NOT NULL default '0',
  unread char(1) NOT NULL default '1',
  content_length int(11) NOT NULL default '0',
  has_attachment char(1) NOT NULL default '0',
  attachment_filename text,
  attachment_content_length int(11) NOT NULL default '0',
  important char(1) NOT NULL default '0',
  system char(1) NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table `notes`
--


--
-- Table structure for table `photos`
--

CREATE TABLE photos (
  ID int(20) unsigned NOT NULL auto_increment,
  filename text,
  orig_filename text,
  descr text,
  sx int(20) default NULL,
  sy int(20) default NULL,
  uid int(20) unsigned NOT NULL default '0',
  size int(10) NOT NULL default '0',
  ext text,
  PRIMARY KEY  (ID),
  KEY uidi (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `photos`
--


--
-- Table structure for table `topics`
--

CREATE TABLE topics (
  channel int(11) NOT NULL default '-1',
  topic blob,
  current_price double NOT NULL default '0',
  orig_price double NOT NULL default '0',
  owner int(11) NOT NULL default '0',
  last_recalc datetime default '1970-01-01 00:00:00',
  disabled char(1) NOT NULL default '0',
  set_time datetime NOT NULL default '0000-00-00 00:00:00',
  KEY __C (channel),
  KEY __LC (last_recalc),
  KEY __D (disabled)
) TYPE=MyISAM;

--
-- Dumping data for table `topics`
--


--
-- Table structure for table `transactions_codes`
--

CREATE TABLE transactions_codes (
  code int(20) NOT NULL default '0',
  descr text NOT NULL,
  PRIMARY KEY  (code)
) TYPE=MyISAM;

--
-- Dumping data for table `transactions_codes`
--


--
-- Table structure for table `user_credits`
--

CREATE TABLE user_credits (
  refid int(10) unsigned NOT NULL default '0',
  summa double NOT NULL default '0',
  last_access datetime default '0000-00-00 00:00:00',
  KEY _refid (refid)
) TYPE=MyISAM;

--
-- Dumping data for table `user_credits`
--


--
-- Table structure for table `user_notes`
--

CREATE TABLE user_notes (
  refid int(11) NOT NULL default '0',
  container blob,
  KEY _zzkZ (refid)
) TYPE=MyISAM;

--
-- Dumping data for table `user_notes`
--


--
-- Table structure for table `user_profiles`
--

CREATE TABLE user_profiles (
  refid int(20) unsigned NOT NULL default '0',
  icq text,
  city text,
  info text,
  homepage text,
  tel text,
  b_day varchar(10) NOT NULL default '',
  b_mon int(3) NOT NULL default '0',
  b_year varchar(4) NOT NULL default '1980',
  fname text,
  lname text,
  email text,
  reg_date datetime default NULL,
  level int(10) unsigned NOT NULL default '100',
  login varchar(18) NOT NULL default '',
  pass text,
  vote_balls int(10) default NULL,
  db_privileges int(10) NOT NULL default '0',
  PRIMARY KEY  (refid),
  KEY _login (login),
  KEY _vb (vote_balls),
  KEY _rd (reg_date),
  KEY _dp (db_privileges),
  KEY lv (level)
) TYPE=MyISAM;

--
-- Dumping data for table `user_profiles`
--


--
-- Table structure for table `user_sets`
--

CREATE TABLE user_sets (
  refid int(20) unsigned NOT NULL default '0',
  pd int(10) NOT NULL default '0',
  lpp int(3) NOT NULL default '0',
  show_nicks char(1) NOT NULL default '1',
  show_level char(1) NOT NULL default '1',
  ngreet int(3) NOT NULL default '0',
  nbye int(3) NOT NULL default '0',
  neye int(3) NOT NULL default '0',
  n_color_pack int(4) NOT NULL default '0',
  settings int(10) NOT NULL default '0',
  color_packs blob,
  replacements blob,
  ignores blob,
  status blob,
  invis_expired datetime NOT NULL default '0000-00-00 00:00:00',
  bought_color_packs blob,
  bought_replacements blob,
  bought_quota int(10) NOT NULL default '0',
  bought_max_nick_count int(10) NOT NULL default '0',
  used_space int(10) NOT NULL default '0',
  quota int(10) NOT NULL default '0',
  max_nick_count int(10) NOT NULL default '0',
  max_msg_len int(10) NOT NULL default '0',
  _privileges int(10) NOT NULL default '0',
  lstate blob,
  gender int(2) NOT NULL default '0',
  levelup_ability int(10) NOT NULL default '0',
  contact_mode char(1) NOT NULL default '0',
  contact_options int(10) unsigned NOT NULL default '0',
  contacts blob,
  invisibility double NOT NULL default '0',
  bought_invisibility double NOT NULL default '0',
  kick_shield double NOT NULL default '0',
  bought_kick_shield double NOT NULL default '0',
  kick_ability double NOT NULL default '0',
  bought_kick_ability double NOT NULL default '0',
  bought_levelup_ability double NOT NULL default '0',
  notes_msg_per_page int(11) NOT NULL default '20',
  notes_save_copy char(1) NOT NULL default '0',
  notes_sort char(1) NOT NULL default '1',
  notes_sort_desc char(1) NOT NULL default '1',
  bought_leveldown_ability double NOT NULL default '0',
  leveldown_ability int(10) NOT NULL default '0',
  poweshen int(11) NOT NULL default '0',
  binv_last_recalc datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (refid),
  KEY __priv (_privileges)
) TYPE=MyISAM;

--
-- Dumping data for table `user_sets`
--


--
-- Table structure for table `user_stats`
--

CREATE TABLE user_stats (
  refid int(20) unsigned NOT NULL auto_increment,
  last_date datetime default NULL,
  last_ip text,
  v_count int(15) default NULL,
  m_count int(10) NOT NULL default '0',
  t_count int(10) NOT NULL default '0',
  kick_count int(10) unsigned NOT NULL default '0',
  kicked_count int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (refid)
) TYPE=MyISAM;

--
-- Dumping data for table `user_stats`
--


--
-- Table structure for table `user_status`
--

CREATE TABLE user_status (
  ID int(20) unsigned NOT NULL auto_increment,
  uid int(20) unsigned NOT NULL default '0',
  name text,
  pic int(20) default NULL,
  disable_invite char(1) NOT NULL default '0',
  uniq char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY uidi (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `user_status`
--


--
-- Table structure for table `users`
--

CREATE TABLE users (
  ID int(10) unsigned NOT NULL auto_increment,
  PRIMARY KEY  (ID)
) TYPE=MyISAM;

--
-- Dumping data for table `users`
--


--
-- Table structure for table `users_count`
--

CREATE TABLE users_count (
  dt datetime NOT NULL default '0000-00-00 00:00:00',
  cnt int(10) NOT NULL default '0',
  PRIMARY KEY  (dt),
  KEY cnt_ (cnt)
) TYPE=MyISAM;

--
-- Dumping data for table `users_count`
--


--
-- Table structure for table `users_fotos`
--

CREATE TABLE users_fotos (
  refid int(20) unsigned NOT NULL default '0',
  fotos blob,
  PRIMARY KEY  (refid)
) TYPE=MyISAM;

--
-- Dumping data for table `users_fotos`
--


--
-- Current Database: chat5
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ chat5;

USE chat5;

--
-- Table structure for table `anekdots`
--

CREATE TABLE anekdots (
  ID int(20) unsigned NOT NULL auto_increment,
  txt text,
  rating int(11) NOT NULL default '0',
  add_date datetime default NULL,
  add_by text,
  chk int(1) NOT NULL default '0',
  ip text,
  PRIMARY KEY  (ID),
  KEY chk_k (chk)
) TYPE=MyISAM;

--
-- Dumping data for table `anekdots`
--


--
-- Table structure for table `banned_ip`
--

CREATE TABLE banned_ip (
  ID int(20) unsigned NOT NULL auto_increment,
  ip text,
  proxy text,
  time_ban datetime default NULL,
  time_free datetime default NULL,
  time_cnt text,
  uid int(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY uidk (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `banned_ip`
--


--
-- Table structure for table `banned_login`
--

CREATE TABLE banned_login (
  ID int(20) unsigned NOT NULL auto_increment,
  login text,
  time_ban datetime default NULL,
  time_free datetime default NULL,
  time_cnt text,
  uid int(20) unsigned NOT NULL default '0',
  descr text NOT NULL,
  PRIMARY KEY  (ID),
  KEY uidk (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `banned_login`
--


--
-- Table structure for table `channels`
--

CREATE TABLE channels (
  ID int(20) unsigned NOT NULL auto_increment,
  censored int(2) default '1',
  save char(1) default '1',
  move_to_channel int(10) default '0',
  move char(1) default '0',
  no_stat char(1) NOT NULL default '0',
  ext char(1) NOT NULL default '0',
  PRIMARY KEY  (ID)
) TYPE=MyISAM;

--
-- Dumping data for table `channels`
--


--
-- Table structure for table `channels_names`
--

CREATE TABLE channels_names (
  ID int(20) unsigned NOT NULL auto_increment,
  cid int(20) unsigned NOT NULL default '0',
  lang char(3) default '',
  name text,
  PRIMARY KEY  (ID),
  KEY cid_key (cid)
) TYPE=MyISAM;

--
-- Dumping data for table `channels_names`
--


--
-- Table structure for table `external_sessions`
--

CREATE TABLE external_sessions (
  login varchar(50) default NULL,
  session_id varchar(50) default NULL,
  KEY __session_id (session_id)
) TYPE=MyISAM;

--
-- Dumping data for table `external_sessions`
--


--
-- Table structure for table `forum_messages`
--

CREATE TABLE forum_messages (
  ID int(20) unsigned NOT NULL auto_increment,
  author varchar(20) default NULL,
  subj text,
  msg text,
  tid int(20) unsigned NOT NULL default '0',
  nid int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  ip varchar(20) default NULL,
  add_date datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (ID),
  KEY uidk (nid),
  KEY nidk (uid),
  KEY tidk (tid),
  KEY add_datek (add_date)
) TYPE=MyISAM;

--
-- Dumping data for table `forum_messages`
--


--
-- Table structure for table `forum_themes`
--

CREATE TABLE forum_themes (
  ID int(20) unsigned NOT NULL auto_increment,
  author varchar(20) default NULL,
  name text,
  nid int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  ip varchar(20) default NULL,
  add_date datetime NOT NULL default '0000-00-00 00:00:00',
  create_date datetime NOT NULL default '0000-00-00 00:00:00',
  msgcnt int(20) NOT NULL default '0',
  last_by varchar(17) NOT NULL default '',
  last_by_id int(20) unsigned default '0',
  PRIMARY KEY  (ID),
  KEY uidk (nid),
  KEY nidk (uid),
  KEY add_datek (add_date)
) TYPE=MyISAM;

--
-- Dumping data for table `forum_themes`
--


--
-- Table structure for table `funnypics`
--

CREATE TABLE funnypics (
  ID int(20) unsigned NOT NULL auto_increment,
  name text,
  descr text,
  showed int(11) NOT NULL default '0',
  rating int(11) NOT NULL default '0',
  PRIMARY KEY  (ID)
) TYPE=MyISAM;

--
-- Dumping data for table `funnypics`
--


--
-- Table structure for table `grant_agrees`
--

CREATE TABLE grant_agrees (
  ID int(20) unsigned NOT NULL auto_increment,
  gid int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  yes int(20) NOT NULL default '0',
  no int(20) NOT NULL default '0',
  login char(20) default NULL,
  PRIMARY KEY  (ID),
  KEY gidk (gid),
  KEY uidk (uid),
  KEY yesk (yes),
  KEY nok (no)
) TYPE=MyISAM;

--
-- Dumping data for table `grant_agrees`
--


--
-- Table structure for table `grant_messages`
--

CREATE TABLE grant_messages (
  ID int(20) unsigned NOT NULL auto_increment,
  gid int(20) unsigned NOT NULL default '0',
  from_nick text,
  ip varchar(20) default NULL,
  msg text,
  add_date datetime default NULL,
  show_nick char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY gidk (gid)
) TYPE=MyISAM;

--
-- Dumping data for table `grant_messages`
--


--
-- Table structure for table `grant_readden`
--

CREATE TABLE grant_readden (
  readden char(1) NOT NULL default '',
  uid int(20) unsigned NOT NULL default '0',
  gid int(20) unsigned NOT NULL default '0',
  KEY uid_key (uid),
  KEY gid_key (gid)
) TYPE=MyISAM;

--
-- Dumping data for table `grant_readden`
--


--
-- Table structure for table `grant_vote`
--

CREATE TABLE grant_vote (
  ID int(20) unsigned NOT NULL auto_increment,
  nick char(20) default NULL,
  llevel int(10) default NULL,
  nlevel int(10) default NULL,
  uid int(20) NOT NULL default '0',
  add_date datetime default NULL,
  add_by int(20) default NULL,
  status char(1) default NULL,
  PRIMARY KEY  (ID),
  KEY uidk (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `grant_vote`
--


--
-- Table structure for table `hints`
--

CREATE TABLE hints (
  ID int(20) unsigned NOT NULL auto_increment,
  txt text NOT NULL,
  lang char(3) NOT NULL default '',
  uid int(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY lang_key (lang),
  KEY uid_key (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `hints`
--


--
-- Table structure for table `histories`
--

CREATE TABLE histories (
  ID int(20) unsigned NOT NULL auto_increment,
  txt text,
  rating int(11) NOT NULL default '0',
  add_date datetime default NULL,
  add_by text,
  chk int(1) NOT NULL default '0',
  ip text,
  PRIMARY KEY  (ID),
  KEY chk_k (chk)
) TYPE=MyISAM;

--
-- Dumping data for table `histories`
--


--
-- Table structure for table `ignores`
--

CREATE TABLE ignores (
  iid int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  KEY id_key (iid),
  KEY uid_key (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `ignores`
--


--
-- Table structure for table `news`
--

CREATE TABLE news (
  ID int(20) unsigned NOT NULL auto_increment,
  uid int(20) unsigned NOT NULL default '0',
  add_date datetime NOT NULL default '0000-00-00 00:00:00',
  lang text,
  txt text,
  add_nid int(20) unsigned default '0',
  PRIMARY KEY  (ID),
  KEY uid_key (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `news`
--


--
-- Table structure for table `nicks`
--

CREATE TABLE nicks (
  ID int(15) unsigned NOT NULL auto_increment,
  Login varchar(18) NOT NULL default '0',
  nick varchar(16) NOT NULL default '',
  uppernick varchar(16) NOT NULL default '',
  Banned int(2) default NULL,
  uid int(10) NOT NULL default '0',
  bby int(10) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY uidk (uid),
  KEY logink (Login),
  KEY bby_key (bby)
) TYPE=MyISAM;

--
-- Dumping data for table `nicks`
--


--
-- Table structure for table `notes`
--

CREATE TABLE notes (
  ID int(20) unsigned NOT NULL auto_increment,
  uid int(20) unsigned NOT NULL default '0',
  from_nick varchar(18) NOT NULL default '',
  from_nick_id int(20) unsigned NOT NULL default '0',
  to_nick varchar(18) NOT NULL default '',
  subj text,
  sent_date datetime NOT NULL default '0000-00-00 00:00:00',
  size int(10) NOT NULL default '0',
  important char(1) NOT NULL default '0',
  status char(1) NOT NULL default '0',
  attach char(1) NOT NULL default '0',
  file_name text,
  file_path text,
  msg text,
  folder int(20) unsigned NOT NULL default '1',
  parent int(20) unsigned NOT NULL default '0',
  system int(2) NOT NULL default '0',
  temporary int(2) NOT NULL default '0',
  file_owner char(1) NOT NULL default '0',
  file_size int(10) unsigned NOT NULL default '0',
  to_nick_id int(20) NOT NULL default '0',
  unread char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY uidi (uid),
  KEY fromi (from_nick),
  KEY from_idi (from_nick_id),
  KEY toi (to_nick),
  KEY sent_datei (sent_date),
  KEY sizei (size),
  KEY impotanti (important),
  KEY statusi (status),
  KEY attachi (attach),
  KEY parenti (parent),
  KEY folderi (folder),
  KEY _unread (unread),
  KEY _to_nick_id (to_nick_id),
  KEY __id (ID),
  KEY __uid (uid),
  KEY _temp (temporary)
) TYPE=MyISAM;

--
-- Dumping data for table `notes`
--


--
-- Table structure for table `notes_filters`
--

CREATE TABLE notes_filters (
  ID int(20) unsigned NOT NULL auto_increment,
  uid int(20) unsigned NOT NULL default '0',
  fid int(20) unsigned NOT NULL default '0',
  to_folder int(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY uid_key (uid),
  KEY fid_key (fid)
) TYPE=MyISAM;

--
-- Dumping data for table `notes_filters`
--


--
-- Table structure for table `notes_folders`
--

CREATE TABLE notes_folders (
  ID int(20) unsigned NOT NULL auto_increment,
  uid int(20) unsigned NOT NULL default '0',
  name char(30) NOT NULL default '',
  show_new char(1) NOT NULL default '1',
  cant_remove char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY cid_key (uid),
  KEY show_new_key (show_new)
) TYPE=MyISAM;

--
-- Dumping data for table `notes_folders`
--


--
-- Table structure for table `photos`
--

CREATE TABLE photos (
  ID int(20) unsigned NOT NULL auto_increment,
  filename text,
  orig_filename text,
  descr text,
  sx int(20) default NULL,
  sy int(20) default NULL,
  uid int(20) unsigned NOT NULL default '0',
  size int(10) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY uidi (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `photos`
--


--
-- Table structure for table `user_credits`
--

CREATE TABLE user_credits (
  refid int(20) unsigned NOT NULL auto_increment,
  summa double NOT NULL default '0',
  last_access datetime default '0000-00-00 00:00:00',
  PRIMARY KEY  (refid),
  KEY summa_key (summa)
) TYPE=MyISAM;

--
-- Dumping data for table `user_credits`
--


--
-- Table structure for table `user_settings`
--

CREATE TABLE user_settings (
  ID int(20) unsigned NOT NULL auto_increment,
  uid int(20) unsigned NOT NULL default '0',
  greeting int(20) default NULL,
  bye int(20) default NULL,
  anick int(10) NOT NULL default '0',
  cstatus int(20) default NULL,
  channel int(20) default NULL,
  show_nicks char(1) NOT NULL default '0',
  show_level char(1) NOT NULL default '1',
  notes_sent_folder int(20) unsigned NOT NULL default '0',
  notes_msg_per_page int(20) unsigned NOT NULL default '20',
  notes_sort_by char(25) NOT NULL default 'sent_date desc',
  notes_save_copy char(1) NOT NULL default '1',
  n_color_pack int(20) default '0',
  settings int(20) unsigned default '0',
  PRIMARY KEY  (ID),
  KEY uidi (uid),
  KEY anick_key (anick)
) TYPE=MyISAM;

--
-- Dumping data for table `user_settings`
--


--
-- Table structure for table `user_status`
--

CREATE TABLE user_status (
  ID int(20) unsigned NOT NULL auto_increment,
  uid int(20) unsigned NOT NULL default '0',
  name text,
  pic int(20) default NULL,
  disable_call char(1) NOT NULL default '0',
  uniq char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY uidi (uid)
) TYPE=MyISAM;

--
-- Dumping data for table `user_status`
--


--
-- Table structure for table `users`
--

CREATE TABLE users (
  ID int(10) unsigned NOT NULL auto_increment,
  Login varchar(18) NOT NULL default '',
  Pass text,
  FirstName text,
  LastName text,
  EMail text,
  ICQ text,
  City text,
  gender char(1) NOT NULL default '0',
  AddInfo text,
  HomePage text,
  BDay char(2) default NULL,
  BMonth char(2) default NULL,
  BYear varchar(4) default NULL,
  RegDate datetime default NULL,
  LastDate datetime default NULL,
  LastIP text,
  Public int(10) default NULL,
  VisCount int(15) default NULL,
  messagecount int(10) NOT NULL default '0',
  timecount int(10) NOT NULL default '0',
  accessmode int(2) NOT NULL default '10',
  LinesPerPage int(15) default NULL,
  Tel text,
  vote_balls int(20) NOT NULL default '0',
  used_space int(10) NOT NULL default '0',
  privs int(10) NOT NULL default '0',
  lang char(3) NOT NULL default '',
  kick int(10) unsigned NOT NULL default '0',
  kicked int(10) unsigned NOT NULL default '0',
  quota int(20) default '0',
  PRIMARY KEY  (ID),
  KEY logink (Login),
  KEY vote_balls_k (vote_balls),
  KEY notes_space_k (used_space),
  KEY messagecount_key (messagecount),
  KEY accessmode_key (accessmode),
  KEY timecount_key (timecount)
) TYPE=MyISAM;

--
-- Dumping data for table `users`
--


--
-- Table structure for table `users_count`
--

CREATE TABLE users_count (
  dt datetime NOT NULL default '0000-00-00 00:00:00',
  cnt int(10) NOT NULL default '0',
  PRIMARY KEY  (dt)
) TYPE=MyISAM;

--
-- Dumping data for table `users_count`
--


--
-- Current Database: datinggizmo
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ datinggizmo;

USE datinggizmo;

--
-- Table structure for table `aboutmydate`
--

CREATE TABLE aboutmydate (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  LookingFor_Gender int(4) NOT NULL default '0',
  LookingFor_From int(4) NOT NULL default '0',
  LookingFor_To int(4) NOT NULL default '0',
  Photos int(4) NOT NULL default '0',
  Tall_FeetFrom int(4) NOT NULL default '0',
  Tall_InchesFrom int(4) NOT NULL default '0',
  Tall_FeetTo int(4) NOT NULL default '0',
  Tall_InchesTo int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `aboutmydate`
--


--
-- Table structure for table `amdBodyType`
--

CREATE TABLE amdBodyType (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdBodyType`
--


--
-- Table structure for table `amdChildren`
--

CREATE TABLE amdChildren (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdChildren`
--


--
-- Table structure for table `amdDrink`
--

CREATE TABLE amdDrink (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdDrink`
--


--
-- Table structure for table `amdEducation`
--

CREATE TABLE amdEducation (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdEducation`
--


--
-- Table structure for table `amdEthnicities`
--

CREATE TABLE amdEthnicities (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdEthnicities`
--


--
-- Table structure for table `amdEyeColor`
--

CREATE TABLE amdEyeColor (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdEyeColor`
--


--
-- Table structure for table `amdFaith`
--

CREATE TABLE amdFaith (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdFaith`
--


--
-- Table structure for table `amdHairColor`
--

CREATE TABLE amdHairColor (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdHairColor`
--


--
-- Table structure for table `amdIncome`
--

CREATE TABLE amdIncome (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdIncome`
--


--
-- Table structure for table `amdJob`
--

CREATE TABLE amdJob (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdJob`
--


--
-- Table structure for table `amdKids`
--

CREATE TABLE amdKids (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdKids`
--


--
-- Table structure for table `amdLanguages`
--

CREATE TABLE amdLanguages (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdLanguages`
--


--
-- Table structure for table `amdRelationship`
--

CREATE TABLE amdRelationship (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdRelationship`
--


--
-- Table structure for table `amdSmoke`
--

CREATE TABLE amdSmoke (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdSmoke`
--


--
-- Table structure for table `cities`
--

CREATE TABLE cities (
  id int(10) unsigned NOT NULL auto_increment,
  city varchar(60) NOT NULL default '',
  state varchar(20) NOT NULL default '',
  zipcode mediumint(6) unsigned NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY zipcode (zipcode),
  KEY city (city)
) TYPE=MyISAM;

--
-- Dumping data for table `cities`
--


--
-- Table structure for table `fieldtypes`
--

CREATE TABLE fieldtypes (
  id bigint(20) unsigned NOT NULL auto_increment,
  name varchar(60) NOT NULL default '',
  type int(4) NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table `fieldtypes`
--


--
-- Table structure for table `images`
--

CREATE TABLE images (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  type enum('P','N') NOT NULL default 'P',
  PRIMARY KEY  (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `images`
--


--
-- Table structure for table `mandatory_fields`
--

CREATE TABLE mandatory_fields (
  id int(10) unsigned NOT NULL auto_increment,
  request_type varchar(45) NOT NULL default '',
  mandatory_field varchar(45) NOT NULL default '',
  PRIMARY KEY  (id),
  KEY request_type (mandatory_field)
) TYPE=MyISAM;

--
-- Dumping data for table `mandatory_fields`
--


--
-- Table structure for table `mpBodyArt`
--

CREATE TABLE mpBodyArt (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpBodyArt`
--


--
-- Table structure for table `mpCommonInterests`
--

CREATE TABLE mpCommonInterests (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpCommonInterests`
--


--
-- Table structure for table `mpDailyDiet`
--

CREATE TABLE mpDailyDiet (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpDailyDiet`
--


--
-- Table structure for table `mpEducation`
--

CREATE TABLE mpEducation (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpEducation`
--


--
-- Table structure for table `mpEthnicities`
--

CREATE TABLE mpEthnicities (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpEthnicities`
--


--
-- Table structure for table `mpHairStyle`
--

CREATE TABLE mpHairStyle (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpHairStyle`
--


--
-- Table structure for table `mpHumorSense`
--

CREATE TABLE mpHumorSense (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpHumorSense`
--


--
-- Table structure for table `mpLanguages`
--

CREATE TABLE mpLanguages (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpLanguages`
--


--
-- Table structure for table `mpLive`
--

CREATE TABLE mpLive (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpLive`
--


--
-- Table structure for table `mpSports`
--

CREATE TABLE mpSports (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpSports`
--


--
-- Table structure for table `profile`
--

CREATE TABLE profile (
  id bigint(20) unsigned NOT NULL auto_increment,
  FirstName varchar(60) NOT NULL default '',
  LastName varchar(60) NOT NULL default '',
  Email varchar(255) NOT NULL default '',
  BirthDate date NOT NULL default '0000-00-00',
  Country varchar(20) NOT NULL default '',
  State varchar(60) NOT NULL default '',
  Zip int(20) NOT NULL default '0',
  City varchar(60) NOT NULL default '',
  last_login int(10) unsigned NOT NULL default '0',
  created date NOT NULL default '0000-00-00',
  modified date NOT NULL default '0000-00-00',
  Relationship int(4) NOT NULL default '0',
  BodyType int(4) NOT NULL default '0',
  Sign int(4) NOT NULL default '0',
  EyeColor int(4) NOT NULL default '0',
  HairColor int(4) NOT NULL default '0',
  BestFeature int(4) NOT NULL default '0',
  Exercise int(4) NOT NULL default '0',
  Smoke int(4) NOT NULL default '0',
  Drink int(4) NOT NULL default '0',
  Job int(4) NOT NULL default '0',
  Income int(4) NOT NULL default '0',
  PoliticalFence int(4) NOT NULL default '0',
  Faith int(4) NOT NULL default '0',
  ProfileName varchar(20) NOT NULL default '',
  Password varchar(20) NOT NULL default '',
  Ultimate int(4) NOT NULL default '0',
  Pets_Dogs int(4) NOT NULL default '0',
  Pets_Cats int(4) NOT NULL default '0',
  Pets_Birds int(4) NOT NULL default '0',
  Pets_Exotic int(4) NOT NULL default '0',
  Pets_Fish int(4) NOT NULL default '0',
  Pets_Gerbils int(4) NOT NULL default '0',
  Pets_Fleas int(4) NOT NULL default '0',
  Pets_Horses int(4) NOT NULL default '0',
  Pets_Reptilies int(4) NOT NULL default '0',
  Pets_Other int(4) NOT NULL default '0',
  Kids_Status int(4) NOT NULL default '0',
  Kids_HowMany int(4) NOT NULL default '0',
  Children_HowMany int(4) NOT NULL default '0',
  Children_Status int(4) NOT NULL default '0',
  Children_Adoption int(4) NOT NULL default '0',
  Gender int(4) NOT NULL default '0',
  Tall_Feet int(4) NOT NULL default '0',
  Tall_Inches int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY zip (Zip)
) TYPE=MyISAM;

--
-- Dumping data for table `profile`
--


--
-- Table structure for table `sessions`
--

CREATE TABLE sessions (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  create_time int(10) NOT NULL default '0',
  sid varchar(40) NOT NULL default '',
  PRIMARY KEY  (id),
  KEY create_time (create_time),
  KEY profile_id (profile_id),
  KEY sid (sid)
) TYPE=MyISAM;

--
-- Dumping data for table `sessions`
--


--
-- Table structure for table `textfields`
--

CREATE TABLE textfields (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  Fun text,
  HotSpots text,
  Things text,
  WasRead text,
  EthnicitiesMore text,
  FaithMore text,
  EducationMore text,
  Yourself text,
  HeadLine text,
  PRIMARY KEY  (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `textfields`
--


--
-- Table structure for table `zipcode`
--

CREATE TABLE zipcode (
  id mediumint(6) unsigned NOT NULL auto_increment,
  zipcode mediumint(6) unsigned NOT NULL default '0',
  latitude double(7,6) NOT NULL default '0.000000',
  longitude double(7,6) NOT NULL default '0.000000',
  PRIMARY KEY  (id),
  KEY zipcode (zipcode)
) TYPE=MyISAM;

--
-- Dumping data for table `zipcode`
--


--
-- Current Database: gizmo
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ gizmo;

USE gizmo;

--
-- Table structure for table `aboutmydate`
--

CREATE TABLE aboutmydate (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  LookingFor_Gender int(4) NOT NULL default '0',
  LookingFor_From int(4) NOT NULL default '0',
  LookingFor_To int(4) NOT NULL default '0',
  Photos int(4) NOT NULL default '0',
  Tall_FeetFrom int(4) NOT NULL default '0',
  Tall_InchesFrom int(4) NOT NULL default '0',
  Tall_FeetTo int(4) NOT NULL default '0',
  Tall_InchesTo int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `aboutmydate`
--


--
-- Table structure for table `amdBodyType`
--

CREATE TABLE amdBodyType (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdBodyType`
--


--
-- Table structure for table `amdChildren`
--

CREATE TABLE amdChildren (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdChildren`
--


--
-- Table structure for table `amdDrink`
--

CREATE TABLE amdDrink (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdDrink`
--


--
-- Table structure for table `amdEducation`
--

CREATE TABLE amdEducation (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdEducation`
--


--
-- Table structure for table `amdEthnicities`
--

CREATE TABLE amdEthnicities (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdEthnicities`
--


--
-- Table structure for table `amdEyeColor`
--

CREATE TABLE amdEyeColor (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdEyeColor`
--


--
-- Table structure for table `amdFaith`
--

CREATE TABLE amdFaith (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdFaith`
--


--
-- Table structure for table `amdHairColor`
--

CREATE TABLE amdHairColor (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdHairColor`
--


--
-- Table structure for table `amdIncome`
--

CREATE TABLE amdIncome (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdIncome`
--


--
-- Table structure for table `amdJob`
--

CREATE TABLE amdJob (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdJob`
--


--
-- Table structure for table `amdKids`
--

CREATE TABLE amdKids (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdKids`
--


--
-- Table structure for table `amdLanguages`
--

CREATE TABLE amdLanguages (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdLanguages`
--


--
-- Table structure for table `amdRelationship`
--

CREATE TABLE amdRelationship (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdRelationship`
--


--
-- Table structure for table `amdSmoke`
--

CREATE TABLE amdSmoke (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `amdSmoke`
--


--
-- Table structure for table `cities`
--

CREATE TABLE cities (
  id int(10) unsigned NOT NULL auto_increment,
  city varchar(60) NOT NULL default '',
  state varchar(20) NOT NULL default '',
  zipcode mediumint(6) unsigned NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY zipcode (zipcode),
  KEY city (city)
) TYPE=MyISAM;

--
-- Dumping data for table `cities`
--


--
-- Table structure for table `fieldtypes`
--

CREATE TABLE fieldtypes (
  id bigint(20) unsigned NOT NULL auto_increment,
  name varchar(60) NOT NULL default '',
  type int(4) NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table `fieldtypes`
--


--
-- Table structure for table `images`
--

CREATE TABLE images (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  type enum('P','N') NOT NULL default 'P',
  PRIMARY KEY  (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `images`
--


--
-- Table structure for table `mandatory_fields`
--

CREATE TABLE mandatory_fields (
  id int(10) unsigned NOT NULL auto_increment,
  request_type varchar(45) NOT NULL default '',
  mandatory_field varchar(45) NOT NULL default '',
  PRIMARY KEY  (id),
  KEY request_type (mandatory_field)
) TYPE=MyISAM;

--
-- Dumping data for table `mandatory_fields`
--


--
-- Table structure for table `mpBodyArt`
--

CREATE TABLE mpBodyArt (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpBodyArt`
--


--
-- Table structure for table `mpCommonInterests`
--

CREATE TABLE mpCommonInterests (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpCommonInterests`
--


--
-- Table structure for table `mpDailyDiet`
--

CREATE TABLE mpDailyDiet (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpDailyDiet`
--


--
-- Table structure for table `mpEducation`
--

CREATE TABLE mpEducation (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpEducation`
--


--
-- Table structure for table `mpEthnicities`
--

CREATE TABLE mpEthnicities (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpEthnicities`
--


--
-- Table structure for table `mpHairStyle`
--

CREATE TABLE mpHairStyle (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpHairStyle`
--


--
-- Table structure for table `mpHumorSense`
--

CREATE TABLE mpHumorSense (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpHumorSense`
--


--
-- Table structure for table `mpLanguages`
--

CREATE TABLE mpLanguages (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpLanguages`
--


--
-- Table structure for table `mpLive`
--

CREATE TABLE mpLive (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpLive`
--


--
-- Table structure for table `mpSports`
--

CREATE TABLE mpSports (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  value int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY id (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `mpSports`
--


--
-- Table structure for table `profile`
--

CREATE TABLE profile (
  id bigint(20) unsigned NOT NULL auto_increment,
  FirstName varchar(60) NOT NULL default '',
  LastName varchar(60) NOT NULL default '',
  Email varchar(255) NOT NULL default '',
  BirthDate date NOT NULL default '0000-00-00',
  Country varchar(20) NOT NULL default '',
  State varchar(60) NOT NULL default '',
  Zip int(20) NOT NULL default '0',
  City varchar(60) NOT NULL default '',
  last_login int(10) unsigned NOT NULL default '0',
  created date NOT NULL default '0000-00-00',
  modified date NOT NULL default '0000-00-00',
  Relationship int(4) NOT NULL default '0',
  BodyType int(4) NOT NULL default '0',
  Sign int(4) NOT NULL default '0',
  EyeColor int(4) NOT NULL default '0',
  HairColor int(4) NOT NULL default '0',
  BestFeature int(4) NOT NULL default '0',
  Exercise int(4) NOT NULL default '0',
  Smoke int(4) NOT NULL default '0',
  Drink int(4) NOT NULL default '0',
  Job int(4) NOT NULL default '0',
  Income int(4) NOT NULL default '0',
  PoliticalFence int(4) NOT NULL default '0',
  Faith int(4) NOT NULL default '0',
  ProfileName varchar(20) NOT NULL default '',
  Password varchar(20) NOT NULL default '',
  Ultimate int(4) NOT NULL default '0',
  Pets_Dogs int(4) NOT NULL default '0',
  Pets_Cats int(4) NOT NULL default '0',
  Pets_Birds int(4) NOT NULL default '0',
  Pets_Exotic int(4) NOT NULL default '0',
  Pets_Fish int(4) NOT NULL default '0',
  Pets_Gerbils int(4) NOT NULL default '0',
  Pets_Fleas int(4) NOT NULL default '0',
  Pets_Horses int(4) NOT NULL default '0',
  Pets_Reptilies int(4) NOT NULL default '0',
  Pets_Other int(4) NOT NULL default '0',
  Kids_Status int(4) NOT NULL default '0',
  Kids_HowMany int(4) NOT NULL default '0',
  Children_HowMany int(4) NOT NULL default '0',
  Children_Status int(4) NOT NULL default '0',
  Children_Adoption int(4) NOT NULL default '0',
  Gender int(4) NOT NULL default '0',
  Tall_Feet int(4) NOT NULL default '0',
  Tall_Inches int(4) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY zip (Zip)
) TYPE=MyISAM;

--
-- Dumping data for table `profile`
--


--
-- Table structure for table `sessions`
--

CREATE TABLE sessions (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  create_time int(10) NOT NULL default '0',
  sid varchar(40) NOT NULL default '',
  PRIMARY KEY  (id),
  KEY create_time (create_time),
  KEY profile_id (profile_id),
  KEY sid (sid)
) TYPE=MyISAM;

--
-- Dumping data for table `sessions`
--


--
-- Table structure for table `textfields`
--

CREATE TABLE textfields (
  id bigint(20) unsigned NOT NULL auto_increment,
  profile_id bigint(20) unsigned NOT NULL default '0',
  Fun text,
  HotSpots text,
  Things text,
  WasRead text,
  EthnicitiesMore text,
  FaithMore text,
  EducationMore text,
  Yourself text,
  HeadLine text,
  PRIMARY KEY  (id),
  KEY profile_id (profile_id)
) TYPE=MyISAM;

--
-- Dumping data for table `textfields`
--


--
-- Table structure for table `zipcode`
--

CREATE TABLE zipcode (
  id mediumint(6) unsigned NOT NULL auto_increment,
  zipcode mediumint(6) unsigned NOT NULL default '0',
  latitude double(7,6) NOT NULL default '0.000000',
  longitude double(7,6) NOT NULL default '0.000000',
  PRIMARY KEY  (id),
  KEY zipcode (zipcode)
) TYPE=MyISAM;

--
-- Dumping data for table `zipcode`
--


--
-- Current Database: mafia
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ mafia;

USE mafia;

--
-- Table structure for table `score`
--

CREATE TABLE score (
  ID int(10) unsigned NOT NULL auto_increment,
  login char(18) NOT NULL default '',
  win int(10) unsigned NOT NULL default '0',
  loose int(10) unsigned NOT NULL default '0',
  score int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY _login (login),
  KEY _win (win),
  KEY _loose (loose),
  KEY _score (score)
) TYPE=ISAM PACK_KEYS=1;

--
-- Dumping data for table `score`
--


--
-- Current Database: mat
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ mat;

USE mat;

--
-- Table structure for table `greetingmsg`
--

CREATE TABLE greetingmsg (
  ID int(10) unsigned NOT NULL auto_increment,
  answer text NOT NULL,
  gender char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY _gender (gender)
) TYPE=ISAM PACK_KEYS=1;

--
-- Dumping data for table `greetingmsg`
--


--
-- Table structure for table `msg`
--

CREATE TABLE msg (
  ID int(10) unsigned NOT NULL auto_increment,
  answer text NOT NULL,
  pattern varchar(100) NOT NULL default '',
  gender char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY _patter (pattern),
  KEY _gender (gender)
) TYPE=MyISAM;

--
-- Dumping data for table `msg`
--


--
-- Table structure for table `privmsg`
--

CREATE TABLE privmsg (
  ID int(10) unsigned NOT NULL auto_increment,
  answer text NOT NULL,
  pattern varchar(100) NOT NULL default '',
  gender char(1) NOT NULL default '0',
  PRIMARY KEY  (ID),
  KEY _patter (pattern),
  KEY _gender (gender)
) TYPE=ISAM PACK_KEYS=1;

--
-- Dumping data for table `privmsg`
--


--
-- Current Database: mysql
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ mysql;

USE mysql;

--
-- Table structure for table `columns_priv`
--

CREATE TABLE columns_priv (
  Host char(60) binary NOT NULL default '',
  Db char(64) binary NOT NULL default '',
  User char(16) binary NOT NULL default '',
  Table_name char(64) binary NOT NULL default '',
  Column_name char(64) binary NOT NULL default '',
  Timestamp timestamp(14) NOT NULL,
  Column_priv set('Select','Insert','Update','References') NOT NULL default '',
  PRIMARY KEY  (Host,Db,User,Table_name,Column_name)
) TYPE=MyISAM COMMENT='Column privileges';

--
-- Dumping data for table `columns_priv`
--


--
-- Table structure for table `db`
--

CREATE TABLE db (
  Host char(60) binary NOT NULL default '',
  Db char(64) binary NOT NULL default '',
  User char(16) binary NOT NULL default '',
  Select_priv enum('N','Y') NOT NULL default 'N',
  Insert_priv enum('N','Y') NOT NULL default 'N',
  Update_priv enum('N','Y') NOT NULL default 'N',
  Delete_priv enum('N','Y') NOT NULL default 'N',
  Create_priv enum('N','Y') NOT NULL default 'N',
  Drop_priv enum('N','Y') NOT NULL default 'N',
  Grant_priv enum('N','Y') NOT NULL default 'N',
  References_priv enum('N','Y') NOT NULL default 'N',
  Index_priv enum('N','Y') NOT NULL default 'N',
  Alter_priv enum('N','Y') NOT NULL default 'N',
  Create_tmp_table_priv enum('N','Y') NOT NULL default 'N',
  Lock_tables_priv enum('N','Y') NOT NULL default 'N',
  PRIMARY KEY  (Host,Db,User),
  KEY User (User)
) TYPE=MyISAM COMMENT='Database privileges';

--
-- Dumping data for table `db`
--


--
-- Table structure for table `func`
--

CREATE TABLE func (
  name char(64) binary NOT NULL default '',
  ret tinyint(1) NOT NULL default '0',
  dl char(128) NOT NULL default '',
  type enum('function','aggregate') NOT NULL default 'function',
  PRIMARY KEY  (name)
) TYPE=MyISAM COMMENT='User defined functions';

--
-- Dumping data for table `func`
--


--
-- Table structure for table `host`
--

CREATE TABLE host (
  Host char(60) binary NOT NULL default '',
  Db char(64) binary NOT NULL default '',
  Select_priv enum('N','Y') NOT NULL default 'N',
  Insert_priv enum('N','Y') NOT NULL default 'N',
  Update_priv enum('N','Y') NOT NULL default 'N',
  Delete_priv enum('N','Y') NOT NULL default 'N',
  Create_priv enum('N','Y') NOT NULL default 'N',
  Drop_priv enum('N','Y') NOT NULL default 'N',
  Grant_priv enum('N','Y') NOT NULL default 'N',
  References_priv enum('N','Y') NOT NULL default 'N',
  Index_priv enum('N','Y') NOT NULL default 'N',
  Alter_priv enum('N','Y') NOT NULL default 'N',
  Create_tmp_table_priv enum('N','Y') NOT NULL default 'N',
  Lock_tables_priv enum('N','Y') NOT NULL default 'N',
  PRIMARY KEY  (Host,Db)
) TYPE=MyISAM COMMENT='Host privileges;  Merged with database privileges';

--
-- Dumping data for table `host`
--


--
-- Table structure for table `tables_priv`
--

CREATE TABLE tables_priv (
  Host char(60) binary NOT NULL default '',
  Db char(64) binary NOT NULL default '',
  User char(16) binary NOT NULL default '',
  Table_name char(64) binary NOT NULL default '',
  Grantor char(77) NOT NULL default '',
  Timestamp timestamp(14) NOT NULL,
  Table_priv set('Select','Insert','Update','Delete','Create','Drop','Grant','References','Index','Alter') NOT NULL default '',
  Column_priv set('Select','Insert','Update','References') NOT NULL default '',
  PRIMARY KEY  (Host,Db,User,Table_name),
  KEY Grantor (Grantor)
) TYPE=MyISAM COMMENT='Table privileges';

--
-- Dumping data for table `tables_priv`
--


--
-- Table structure for table `user`
--

CREATE TABLE user (
  Host varchar(60) binary NOT NULL default '',
  User varchar(16) binary NOT NULL default '',
  Password varchar(16) binary NOT NULL default '',
  Select_priv enum('N','Y') NOT NULL default 'N',
  Insert_priv enum('N','Y') NOT NULL default 'N',
  Update_priv enum('N','Y') NOT NULL default 'N',
  Delete_priv enum('N','Y') NOT NULL default 'N',
  Create_priv enum('N','Y') NOT NULL default 'N',
  Drop_priv enum('N','Y') NOT NULL default 'N',
  Reload_priv enum('N','Y') NOT NULL default 'N',
  Shutdown_priv enum('N','Y') NOT NULL default 'N',
  Process_priv enum('N','Y') NOT NULL default 'N',
  File_priv enum('N','Y') NOT NULL default 'N',
  Grant_priv enum('N','Y') NOT NULL default 'N',
  References_priv enum('N','Y') NOT NULL default 'N',
  Index_priv enum('N','Y') NOT NULL default 'N',
  Alter_priv enum('N','Y') NOT NULL default 'N',
  Show_db_priv enum('N','Y') NOT NULL default 'N',
  Super_priv enum('N','Y') NOT NULL default 'N',
  Create_tmp_table_priv enum('N','Y') NOT NULL default 'N',
  Lock_tables_priv enum('N','Y') NOT NULL default 'N',
  Execute_priv enum('N','Y') NOT NULL default 'N',
  Repl_slave_priv enum('N','Y') NOT NULL default 'N',
  Repl_client_priv enum('N','Y') NOT NULL default 'N',
  ssl_type enum('','ANY','X509','SPECIFIED') NOT NULL default '',
  ssl_cipher blob NOT NULL,
  x509_issuer blob NOT NULL,
  x509_subject blob NOT NULL,
  max_questions int(11) unsigned NOT NULL default '0',
  max_updates int(11) unsigned NOT NULL default '0',
  max_connections int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (Host,User)
) TYPE=MyISAM COMMENT='Users and global privileges';

--
-- Dumping data for table `user`
--


--
-- Current Database: quiz
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ quiz;

USE quiz;

--
-- Table structure for table `questions`
--

CREATE TABLE questions (
  ID int(10) unsigned NOT NULL auto_increment,
  question text NOT NULL,
  answer text NOT NULL,
  PRIMARY KEY  (ID)
) TYPE=MyISAM;

--
-- Dumping data for table `questions`
--


--
-- Table structure for table `rating`
--

CREATE TABLE rating (
  login char(18) NOT NULL default '',
  score int(10) NOT NULL default '0',
  KEY _login (login)
) TYPE=MyISAM;

--
-- Dumping data for table `rating`
--


--
-- Current Database: test
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ test;

USE test;

