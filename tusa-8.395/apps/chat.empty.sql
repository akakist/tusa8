-- MySQL dump 9.07
--
-- Host: localhost    Database: chat
---------------------------------------------------------
-- Server version	4.0.12

--
-- Table structure for table 'anekdots'
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
-- Dumping data for table 'anekdots'
--


--
-- Table structure for table 'banned_ip'
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
-- Dumping data for table 'banned_ip'
--


--
-- Table structure for table 'banned_login'
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
-- Dumping data for table 'banned_login'
--


--
-- Table structure for table 'bx_advertizers'
--

CREATE TABLE bx_advertizers (
  refid int(20) unsigned NOT NULL default '0',
  remained_sum double(16,4) NOT NULL default '0.0000',
  shown_total_sum double(16,4) NOT NULL default '0.0000',
  credit_limit_sum double(16,4) NOT NULL default '0.0000',
  payed_sum double(16,4) NOT NULL default '0.0000',
  auto_allow char(1) NOT NULL default '0',
  min_price double NOT NULL default '1',
  ponds double default '0',
  last_system_selected int(20) unsigned NOT NULL default '0',
  agreement_accepted char(1) NOT NULL default '0',
  KEY _refid (refid)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_advertizers'
--


--
-- Table structure for table 'bx_banners'
--

CREATE TABLE bx_banners (
  id int(20) unsigned NOT NULL auto_increment,
  campaign int(20) unsigned NOT NULL default '0',
  system int(20) unsigned NOT NULL default '0',
  name varchar(50) NOT NULL default '',
  url text,
  alt text,
  descr text,
  path text,
  allowed_by_admin char(1) NOT NULL default '0',
  checked_by_admin char(1) NOT NULL default '0',
  deleted char(1) NOT NULL default '0',
  decline_reason text,
  exchange_only char(1) NOT NULL default '0',
  site_ref int(20) unsigned NOT NULL default '0',
  type char(3) NOT NULL default 'exc',
  total_shown_N int(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY _owner (campaign),
  KEY _deleted (deleted)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_banners'
--


--
-- Table structure for table 'bx_campaigns'
--

CREATE TABLE bx_campaigns (
  id int(20) unsigned NOT NULL auto_increment,
  owner int(20) unsigned NOT NULL default '0',
  name varchar(50) NOT NULL default '',
  descr text,
  deleted char(1) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY _owner (owner),
  KEY _deleted (deleted)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_campaigns'
--


--
-- Table structure for table 'bx_groups'
--

CREATE TABLE bx_groups (
  id int(20) unsigned NOT NULL auto_increment,
  name text,
  descr text,
  deleted char(1) NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_groups'
--


--
-- Table structure for table 'bx_money_transactions'
--

CREATE TABLE bx_money_transactions (
  id int(20) unsigned NOT NULL auto_increment,
  summa double(16,4) NOT NULL default '0.0000',
  from_user int(20) unsigned NOT NULL default '0',
  to_user int(20) unsigned NOT NULL default '0',
  committed char(1) NOT NULL default '0',
  create_time datetime default NULL,
  commit_time datetime default NULL,
  PRIMARY KEY  (id),
  KEY _fu (from_user),
  KEY _tu (to_user)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_money_transactions'
--


--
-- Table structure for table 'bx_orders'
--

CREATE TABLE bx_orders (
  id int(20) unsigned NOT NULL auto_increment,
  banner int(20) unsigned NOT NULL default '0',
  name text,
  descr text,
  cost_coeff double(4,2) NOT NULL default '0.00',
  date_targeting text,
  week_targeting text,
  day_targeting text,
  host_targeting text,
  gender_targeting char(1) NOT NULL default '0',
  user_packet_size int(8) unsigned NOT NULL default '0',
  user_packet_interval int(8) unsigned NOT NULL default '0',
  user_packet_exterval int(8) unsigned NOT NULL default '0',
  max_shows_total int(8) unsigned NOT NULL default '0',
  deleted char(1) NOT NULL default '0',
  total_shown_N int(10) unsigned NOT NULL default '0',
  total_shown_cost double(16,4) NOT NULL default '0.0000',
  adv_places_allowed text,
  adv_places_banned text,
  adv_groups_allowed text,
  adv_groups_banned text,
  max_price double NOT NULL default '0',
  enabled char(1) NOT NULL default '0',
  stop_after_click char(1) NOT NULL default '1',
  hour_limit int(20) unsigned NOT NULL default '0',
  day_limit int(20) unsigned NOT NULL default '0',
  is_default char(1) NOT NULL default '0',
  places blob,
  PRIMARY KEY  (id),
  KEY _banner (banner)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_orders'
--


--
-- Table structure for table 'bx_places'
--

CREATE TABLE bx_places (
  id int(20) unsigned NOT NULL auto_increment,
  system int(20) unsigned NOT NULL default '0',
  site int(20) unsigned NOT NULL default '0',
  name text,
  descr text,
  page_url text,
  cost_1000_shows double NOT NULL default '0',
  deleted char(1) NOT NULL default '0',
  checked_by_admin char(1) NOT NULL default '0',
  allowed_by_admin char(1) NOT NULL default '0',
  decline_reason text,
  can_change_gender char(1) NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_places'
--


--
-- Table structure for table 'bx_sites'
--

CREATE TABLE bx_sites (
  id int(20) unsigned NOT NULL auto_increment,
  owner int(20) unsigned NOT NULL default '0',
  name text,
  descr text,
  url text,
  deleted char(1) NOT NULL default '0',
  checked_by_admin char(1) NOT NULL default '0',
  allowed_by_admin char(1) NOT NULL default '0',
  groups blob,
  decline_reason text,
  ponds blob,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_sites'
--


--
-- Table structure for table 'bx_systems'
--

CREATE TABLE bx_systems (
  id int(20) unsigned NOT NULL auto_increment,
  width int(20) unsigned NOT NULL default '0',
  height int(20) unsigned NOT NULL default '0',
  name text,
  descr text,
  deleted char(1) NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table 'bx_systems'
--


--
-- Table structure for table 'channels'
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
  PRIMARY KEY  (ID)
) TYPE=MyISAM;

--
-- Dumping data for table 'channels'
--


--
-- Table structure for table 'channels_names'
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
-- Dumping data for table 'channels_names'
--


--
-- Table structure for table 'credits_transactions'
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
-- Dumping data for table 'credits_transactions'
--


--
-- Table structure for table 'forum_messages'
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
-- Dumping data for table 'forum_messages'
--


--
-- Table structure for table 'forum_themes'
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
-- Dumping data for table 'forum_themes'
--


--
-- Table structure for table 'funnypics'
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
-- Dumping data for table 'funnypics'
--


--
-- Table structure for table 'grant_agrees'
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
-- Dumping data for table 'grant_agrees'
--


--
-- Table structure for table 'grant_messages'
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
-- Dumping data for table 'grant_messages'
--


--
-- Table structure for table 'grant_readden'
--

CREATE TABLE grant_readden (
  readden char(1) NOT NULL default '',
  uid int(20) unsigned NOT NULL default '0',
  gid int(20) unsigned NOT NULL default '0',
  KEY uid_key (uid),
  KEY gid_key (gid)
) TYPE=MyISAM;

--
-- Dumping data for table 'grant_readden'
--


--
-- Table structure for table 'grant_vote'
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
-- Dumping data for table 'grant_vote'
--


--
-- Table structure for table 'hints'
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
-- Dumping data for table 'hints'
--


--
-- Table structure for table 'histories'
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
-- Dumping data for table 'histories'
--


--
-- Table structure for table 'ignores'
--

CREATE TABLE ignores (
  iid int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  KEY id_key (iid),
  KEY uid_key (uid)
) TYPE=MyISAM;

--
-- Dumping data for table 'ignores'
--


--
-- Table structure for table 'news'
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
-- Dumping data for table 'news'
--


--
-- Table structure for table 'nicks'
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
-- Dumping data for table 'nicks'
--


--
-- Table structure for table 'notes'
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
  KEY sent_datei (sent_date),
  KEY attachi (attach),
  KEY folderi (folder),
  KEY _unread (unread),
  KEY _temp (temporary)
) TYPE=MyISAM;

--
-- Dumping data for table 'notes'
--


--
-- Table structure for table 'notes_filters'
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
-- Dumping data for table 'notes_filters'
--


--
-- Table structure for table 'notes_folders'
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
-- Dumping data for table 'notes_folders'
--


--
-- Table structure for table 'photos'
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
-- Dumping data for table 'photos'
--


--
-- Table structure for table 'sent_sms'
--

CREATE TABLE sent_sms (
  id int(20) unsigned NOT NULL default '0',
  uid int(20) unsigned NOT NULL default '0',
  msg text,
  status int(10) default NULL,
  phone varchar(17) default NULL,
  dt datetime default NULL,
  PRIMARY KEY  (id),
  KEY _uid (uid),
  KEY _phone (phone),
  KEY _dt (dt),
  KEY _status (status)
) TYPE=MyISAM;

--
-- Dumping data for table 'sent_sms'
--


--
-- Table structure for table 'transactions_codes'
--

CREATE TABLE transactions_codes (
  code int(20) NOT NULL default '0',
  descr text NOT NULL,
  PRIMARY KEY  (code)
) TYPE=MyISAM;

--
-- Dumping data for table 'transactions_codes'
--


--
-- Table structure for table 'user_credits'
--

CREATE TABLE user_credits (
  refid int(20) unsigned NOT NULL auto_increment,
  summa double NOT NULL default '0',
  last_access datetime default '0000-00-00 00:00:00',
  PRIMARY KEY  (refid),
  KEY summa_key (summa)
) TYPE=MyISAM;

--
-- Dumping data for table 'user_credits'
--


--
-- Table structure for table 'user_settings'
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
-- Dumping data for table 'user_settings'
--


--
-- Table structure for table 'user_status'
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
-- Dumping data for table 'user_status'
--


--
-- Table structure for table 'users'
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
  bought_quota int(10) NOT NULL default '0',
  bought_max_nick_count int(10) NOT NULL default '0',
  invis_bought char(1) NOT NULL default '0',
  invis_bought_expired datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (ID),
  KEY vote_balls_k (vote_balls),
  KEY notes_space_k (used_space),
  KEY messagecount_key (messagecount),
  KEY accessmode_key (accessmode),
  KEY timecount_key (timecount),
  KEY logink (Login)
) TYPE=MyISAM;

--
-- Dumping data for table 'users'
--


--
-- Table structure for table 'users_count'
--

CREATE TABLE users_count (
  dt datetime NOT NULL default '0000-00-00 00:00:00',
  cnt int(10) NOT NULL default '0',
  PRIMARY KEY  (dt),
  KEY cnt_ (cnt)
) TYPE=MyISAM;

--
-- Dumping data for table 'users_count'
--


