-- MySQL dump 9.07
--
-- Host: localhost    Database: mafia
---------------------------------------------------------
-- Server version	4.0.12

--
-- Table structure for table 'score'
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
-- Dumping data for table 'score'
--

INSERT INTO score VALUES (475,'1=TIGRICA=',0,1,59);
INSERT INTO score VALUES (474,'ShadoVV',1,0,35);
INSERT INTO score VALUES (473,'Spot-XOu',0,1,104);
INSERT INTO score VALUES (472,'Molex',0,2,130);
INSERT INTO score VALUES (471,'graft',0,1,0);
INSERT INTO score VALUES (470,'BIOZ',0,1,0);
INSERT INTO score VALUES (469,'Aliwera',0,1,106);
INSERT INTO score VALUES (468,'FLINA',0,2,70);
INSERT INTO score VALUES (467,'novostroyniy',1,0,50);
INSERT INTO score VALUES (466,'Zomor',0,1,21);
INSERT INTO score VALUES (465,'Pawel_OS',0,1,25);
INSERT INTO score VALUES (464,'siemens-5',0,1,129);
INSERT INTO score VALUES (463,'$hrek',1,2,99);
INSERT INTO score VALUES (462,'Raiser',0,2,10);
INSERT INTO score VALUES (461,'GROG123321',1,0,116);
INSERT INTO score VALUES (460,'Spasatel',0,1,0);
INSERT INTO score VALUES (459,'suslik_newss',0,2,0);
INSERT INTO score VALUES (458,'Berdan',0,1,38);
INSERT INTO score VALUES (457,'REQION',8,26,977);
INSERT INTO score VALUES (456,'Glukalo',0,4,121);
INSERT INTO score VALUES (455,'FS',0,1,0);
INSERT INTO score VALUES (454,'Erida',0,3,53);
INSERT INTO score VALUES (453,'zubra',0,1,0);
INSERT INTO score VALUES (452,'999359',0,5,31);
INSERT INTO score VALUES (451,'fagre',0,1,0);
INSERT INTO score VALUES (450,'940',0,1,0);
INSERT INTO score VALUES (449,'005',0,1,105);
INSERT INTO score VALUES (448,'_KamiKadZe_',0,3,262);
INSERT INTO score VALUES (447,'ctwin',0,2,246);
INSERT INTO score VALUES (446,'29121986',1,2,506);
INSERT INTO score VALUES (445,'Titowa',0,1,0);
INSERT INTO score VALUES (444,'Woland',0,3,10);
INSERT INTO score VALUES (443,'_Dron_',1,5,626);
INSERT INTO score VALUES (442,'vka1411',1,1,73);
INSERT INTO score VALUES (441,'13021989',1,8,157);
INSERT INTO score VALUES (440,'SPLIN',0,1,0);
INSERT INTO score VALUES (439,'kusmas',0,1,30);
INSERT INTO score VALUES (438,'sya_1',0,3,220);
INSERT INTO score VALUES (437,'1357913',1,0,50);
INSERT INTO score VALUES (436,'forest',2,11,579);
INSERT INTO score VALUES (435,'Aksen1',0,1,0);
INSERT INTO score VALUES (434,'Quiet2',3,10,477);
INSERT INTO score VALUES (433,'__VAMPIRO__',18,25,2184);
INSERT INTO score VALUES (432,'gora',0,1,190);
INSERT INTO score VALUES (431,'Mata_Hari',8,25,2641);
INSERT INTO score VALUES (430,'Frolova',0,1,60);
INSERT INTO score VALUES (429,'Bokser2003',2,1,290);
INSERT INTO score VALUES (428,'Lelikcool',0,1,48);
INSERT INTO score VALUES (427,'COLT',1,1,300);
INSERT INTO score VALUES (426,'Jester',0,1,25);
INSERT INTO score VALUES (425,'ADOBE',9,16,1169);
INSERT INTO score VALUES (424,'*Folga*',0,3,74);
INSERT INTO score VALUES (423,'Weiss',3,11,623);
INSERT INTO score VALUES (422,'restart',2,1,355);
INSERT INTO score VALUES (421,'tomichka',0,1,101);
INSERT INTO score VALUES (420,'lodypb',2,3,110);
INSERT INTO score VALUES (419,'LEXER__',0,2,12);
INSERT INTO score VALUES (418,'mazatraker',1,3,222);
INSERT INTO score VALUES (417,'kc',1,7,267);
INSERT INTO score VALUES (416,'ZHoRiCK',4,7,1044);
INSERT INTO score VALUES (415,'ins',0,1,0);
INSERT INTO score VALUES (414,'tcpip',0,1,99);
INSERT INTO score VALUES (413,'Detsl',1,2,218);
INSERT INTO score VALUES (412,'Venus',3,7,411);
INSERT INTO score VALUES (411,'PTvision',0,1,0);
INSERT INTO score VALUES (410,'Egor777',0,1,0);
INSERT INTO score VALUES (409,'Canyon',9,18,1082);
INSERT INTO score VALUES (408,'CE6EJlEB',0,3,64);
INSERT INTO score VALUES (407,'DANON',3,2,319);
INSERT INTO score VALUES (406,'Sony_77',0,2,65);
INSERT INTO score VALUES (405,'Bpa4',0,1,45);
INSERT INTO score VALUES (404,'Skin_head',0,2,73);
INSERT INTO score VALUES (403,'252627',0,1,0);
INSERT INTO score VALUES (402,'beelain',0,2,181);
INSERT INTO score VALUES (401,'NRomashka',1,0,249);
INSERT INTO score VALUES (400,'lenasha',1,11,671);
INSERT INTO score VALUES (399,'kattt',0,1,0);
INSERT INTO score VALUES (398,'table',7,18,1138);
INSERT INTO score VALUES (397,'Vitalya',2,9,550);
INSERT INTO score VALUES (396,'Golinikii',0,2,37);
INSERT INTO score VALUES (395,'agata',5,9,935);
INSERT INTO score VALUES (394,'KaRaTeLb',1,3,0);
INSERT INTO score VALUES (393,'BLack_Smak',1,1,47);
INSERT INTO score VALUES (392,'ramm',1,1,153);
INSERT INTO score VALUES (391,'Stack51',1,2,526);
INSERT INTO score VALUES (390,'Don',1,2,402);
INSERT INTO score VALUES (389,'=Pred=',1,8,497);
INSERT INTO score VALUES (388,'rentv',0,1,12);
INSERT INTO score VALUES (387,'4umovoz',2,3,573);
INSERT INTO score VALUES (386,'solnishko',1,5,269);
INSERT INTO score VALUES (385,'Alia_lia',0,6,154);
INSERT INTO score VALUES (384,'Darkside',0,1,49);
INSERT INTO score VALUES (383,'22222222222222',1,1,312);
INSERT INTO score VALUES (382,'ADC',3,8,849);
INSERT INTO score VALUES (381,'AAAandrey',0,3,65);
INSERT INTO score VALUES (380,'Stylet',6,11,1427);
INSERT INTO score VALUES (379,'jimbo',0,2,60);
INSERT INTO score VALUES (378,'7777777777',2,13,132);
INSERT INTO score VALUES (377,'RU_DEN',2,12,573);
INSERT INTO score VALUES (376,'Sonce',5,7,732);
INSERT INTO score VALUES (375,'Smoller',0,2,72);
INSERT INTO score VALUES (374,'-SHUM-',4,4,472);
INSERT INTO score VALUES (373,'icicle',0,7,293);
INSERT INTO score VALUES (372,'chitter',1,9,516);
INSERT INTO score VALUES (371,'CTIZEN',1,1,0);
INSERT INTO score VALUES (370,'omfal',1,1,139);
INSERT INTO score VALUES (369,'Zoolog',5,18,1779);
INSERT INTO score VALUES (368,'Flameonah',0,1,53);
INSERT INTO score VALUES (367,'Arhon13',0,1,0);
INSERT INTO score VALUES (366,'Golum',1,3,541);
INSERT INTO score VALUES (365,'DJobo',2,12,666);
INSERT INTO score VALUES (364,'req',0,1,93);
INSERT INTO score VALUES (363,'-open-',0,1,60);
INSERT INTO score VALUES (362,'rawer',1,12,246);
INSERT INTO score VALUES (361,'tutoir',1,8,612);
INSERT INTO score VALUES (360,'Sex_bomba',1,1,35);
INSERT INTO score VALUES (359,'GYG',10,64,3896);
INSERT INTO score VALUES (358,'Pha_1',1,4,311);
INSERT INTO score VALUES (357,'NIKOss',0,1,0);
INSERT INTO score VALUES (356,'shoker',0,3,114);
INSERT INTO score VALUES (355,'xxxl',1,2,204);
INSERT INTO score VALUES (354,'GhouL',0,1,0);
INSERT INTO score VALUES (353,'71717',2,9,419);
INSERT INTO score VALUES (352,'Batrak',10,14,1871);
INSERT INTO score VALUES (351,'Russian-Forum',23,92,5909);
INSERT INTO score VALUES (350,'WILD',27,36,3871);
INSERT INTO score VALUES (349,'6TP',0,7,82);
INSERT INTO score VALUES (348,'moniak',8,12,807);
INSERT INTO score VALUES (347,'Daifs',6,10,782);
INSERT INTO score VALUES (346,'Otdel',6,11,827);
INSERT INTO score VALUES (345,'maximka_max',0,1,12);
INSERT INTO score VALUES (344,'_SNAP_',4,3,559);
INSERT INTO score VALUES (343,'Raksa',0,2,12);
INSERT INTO score VALUES (342,'Knightmare',31,47,7475);
INSERT INTO score VALUES (341,'wer',0,2,105);
INSERT INTO score VALUES (340,'fyz',0,1,24);
INSERT INTO score VALUES (339,'zmeika',0,4,0);
INSERT INTO score VALUES (338,'asdfghz',10,35,2490);
INSERT INTO score VALUES (337,'SHARKI',0,1,0);
INSERT INTO score VALUES (336,'13131',0,1,48);
INSERT INTO score VALUES (335,'Skippy',2,14,562);
INSERT INTO score VALUES (334,'bombochka',0,1,53);
INSERT INTO score VALUES (333,'demon_737',14,74,6269);
INSERT INTO score VALUES (332,'Alive',1,4,307);
INSERT INTO score VALUES (331,'DevilMoon',1,2,58);
INSERT INTO score VALUES (330,'Alter',0,1,5);
INSERT INTO score VALUES (329,'TARZ',7,50,2037);
INSERT INTO score VALUES (328,'babai99',25,63,5593);
INSERT INTO score VALUES (327,'NAZZLO',0,7,140);
INSERT INTO score VALUES (326,'LUAZ',8,15,1591);
INSERT INTO score VALUES (325,'555666',4,4,458);
INSERT INTO score VALUES (324,'HRS',3,1,308);
INSERT INTO score VALUES (323,'*molchun*',0,5,63);
INSERT INTO score VALUES (322,'Alex0',5,5,649);
INSERT INTO score VALUES (321,'wraith',0,2,71);
INSERT INTO score VALUES (320,'molchun',0,2,0);
INSERT INTO score VALUES (319,'IRISSKA',2,1,200);
INSERT INTO score VALUES (318,'00-00-00',0,6,46);
INSERT INTO score VALUES (317,'KIRPICH2',0,1,0);
INSERT INTO score VALUES (316,'Dash_switch',2,3,237);
INSERT INTO score VALUES (315,'696100',1,0,236);
INSERT INTO score VALUES (314,'FOX_MALDER',1,6,335);
INSERT INTO score VALUES (313,'krot2',3,9,800);
INSERT INTO score VALUES (312,'vdxbv',1,3,505);
INSERT INTO score VALUES (311,'pork',1,1,360);
INSERT INTO score VALUES (310,'Svetlana',2,12,987);
INSERT INTO score VALUES (309,'DEFF',0,5,367);
INSERT INTO score VALUES (308,'WARC',0,1,71);
INSERT INTO score VALUES (307,'wlad',0,4,226);
INSERT INTO score VALUES (306,'0688',0,1,20);
INSERT INTO score VALUES (305,'_Goblin_away',0,3,273);
INSERT INTO score VALUES (304,'Friker_666',14,40,3883);
INSERT INTO score VALUES (303,'spell',27,26,3829);
INSERT INTO score VALUES (302,'_Respect_',43,60,9691);
INSERT INTO score VALUES (301,'Tihonees',0,1,40);
INSERT INTO score VALUES (300,'34367789',4,9,928);
INSERT INTO score VALUES (299,'_spirit',0,1,125);
INSERT INTO score VALUES (298,'CHEGOL',0,2,53);
INSERT INTO score VALUES (297,'20001',1,1,493);
INSERT INTO score VALUES (296,'RYTIN',1,0,242);
INSERT INTO score VALUES (295,'BAHO771578',0,6,271);
INSERT INTO score VALUES (294,'h1m3rka',2,10,1025);
INSERT INTO score VALUES (293,'Virto',3,14,649);
INSERT INTO score VALUES (292,'gogita',0,1,136);
INSERT INTO score VALUES (291,'VEK',0,4,158);
INSERT INTO score VALUES (290,'kazak',0,2,0);
INSERT INTO score VALUES (289,'cnfcmrf',5,4,730);
INSERT INTO score VALUES (288,'DUX',0,2,5);
INSERT INTO score VALUES (287,'SWay',2,3,246);
INSERT INTO score VALUES (286,'TeamKiller',1,15,940);
INSERT INTO score VALUES (285,'Tapka',8,9,1132);
INSERT INTO score VALUES (284,'DENissssssss',19,16,2385);
INSERT INTO score VALUES (283,'_Slayer_',3,9,551);
INSERT INTO score VALUES (282,'$AHTOXA$',0,1,12);
INSERT INTO score VALUES (281,'-=IIIyT=-',4,0,200);
INSERT INTO score VALUES (280,'PHENIKS',8,1,511);
INSERT INTO score VALUES (279,'Negative',26,16,2855);
INSERT INTO score VALUES (278,'Search',0,3,125);
INSERT INTO score VALUES (277,'pol',0,2,85);
INSERT INTO score VALUES (276,'rich45',0,1,14);
INSERT INTO score VALUES (275,'MNBV',0,2,0);
INSERT INTO score VALUES (274,'lips',1,0,301);
INSERT INTO score VALUES (273,'tuhiy',3,4,375);
INSERT INTO score VALUES (272,'Jenka_EA',41,64,8503);
INSERT INTO score VALUES (271,'lo_h',2,1,436);
INSERT INTO score VALUES (270,'Vse_pytem',0,12,683);
INSERT INTO score VALUES (269,'jazzmen',6,35,2267);
INSERT INTO score VALUES (268,'5454',1,1,109);
INSERT INTO score VALUES (267,'lisorub',2,23,1728);
INSERT INTO score VALUES (266,'dipper',3,9,1197);
INSERT INTO score VALUES (265,'DDI',2,14,1013);
INSERT INTO score VALUES (264,'Zfed',13,25,2609);
INSERT INTO score VALUES (263,'Maks_T',0,1,0);
INSERT INTO score VALUES (262,'RistaR',4,9,780);
INSERT INTO score VALUES (261,'Mishan',1,4,360);
INSERT INTO score VALUES (260,'Dgfjh',5,12,1161);
INSERT INTO score VALUES (259,'Chainik',6,9,1414);
INSERT INTO score VALUES (258,'loovez',3,5,352);
INSERT INTO score VALUES (257,'Lilu16',7,14,1472);
INSERT INTO score VALUES (256,'BIZON666',8,17,1409);
INSERT INTO score VALUES (255,'jonnu',1,0,115);
INSERT INTO score VALUES (254,'Untouchable',20,31,5760);
INSERT INTO score VALUES (253,'juliak',1,6,426);
INSERT INTO score VALUES (252,'HMZ',3,9,928);
INSERT INTO score VALUES (251,'Dari',15,31,2169);
INSERT INTO score VALUES (250,'artemka',2,0,425);
INSERT INTO score VALUES (249,'Druz',17,19,4260);
INSERT INTO score VALUES (248,'*GOLDBERG',0,1,24);
INSERT INTO score VALUES (247,'150390',0,1,47);
INSERT INTO score VALUES (246,'=tujuj8i',9,19,2421);
INSERT INTO score VALUES (245,'Poal',0,3,63);
INSERT INTO score VALUES (244,'ShurikD',23,88,7451);
INSERT INTO score VALUES (243,'GANGBOY',2,6,465);
INSERT INTO score VALUES (242,'Monach',1,3,181);
INSERT INTO score VALUES (241,'2005*',1,3,147);
INSERT INTO score VALUES (240,'roma88',0,1,0);
INSERT INTO score VALUES (239,'Digger',0,1,0);
INSERT INTO score VALUES (238,'MOLNIA-A',1,1,35);
INSERT INTO score VALUES (237,'nike_',1,0,50);
INSERT INTO score VALUES (236,'*=LEXX_2=*',0,2,14);
INSERT INTO score VALUES (235,'_ash',1,3,196);
INSERT INTO score VALUES (234,'geezo',0,2,93);
INSERT INTO score VALUES (233,'338',14,13,1089);
INSERT INTO score VALUES (232,'Claws',1,0,50);
INSERT INTO score VALUES (231,'monstr',0,5,105);
INSERT INTO score VALUES (230,'zoomko',13,43,2083);
INSERT INTO score VALUES (229,'konfetka',0,1,106);
INSERT INTO score VALUES (228,'Blind',2,2,338);
INSERT INTO score VALUES (227,'Arick',0,1,10);
INSERT INTO score VALUES (226,'Prostopahan',0,7,284);
INSERT INTO score VALUES (225,'BORYGA',5,9,1013);
INSERT INTO score VALUES (224,'_U_L_A_',19,18,3147);
INSERT INTO score VALUES (223,'Hruk',11,27,2706);
INSERT INTO score VALUES (222,'Muri',20,32,5065);
INSERT INTO score VALUES (221,'GNOMUs',5,8,1083);
INSERT INTO score VALUES (220,'456217',1,0,103);
INSERT INTO score VALUES (219,'winder',3,15,938);
INSERT INTO score VALUES (218,'Batman7',3,29,1781);
INSERT INTO score VALUES (217,'wrestling',0,4,117);
INSERT INTO score VALUES (216,'Ministrrr',3,10,1223);
INSERT INTO score VALUES (215,'_Status_',1,0,88);
INSERT INTO score VALUES (214,'Papik',0,1,124);
INSERT INTO score VALUES (213,'Silver_Drakon',0,1,12);
INSERT INTO score VALUES (212,'sergey555',1,2,510);
INSERT INTO score VALUES (211,'SMS',0,5,24);
INSERT INTO score VALUES (210,'asivi',0,1,0);
INSERT INTO score VALUES (209,'*_Flameon_*',9,39,2058);
INSERT INTO score VALUES (208,'210721',0,1,38);
INSERT INTO score VALUES (207,'$',0,1,0);
INSERT INTO score VALUES (206,'lumo',4,2,714);
INSERT INTO score VALUES (205,'130391',0,3,102);
INSERT INTO score VALUES (204,'Vano_007',0,2,61);
INSERT INTO score VALUES (203,'Orleynok',1,2,257);
INSERT INTO score VALUES (202,'liana2003',1,2,397);
INSERT INTO score VALUES (201,'29140688',4,8,675);
INSERT INTO score VALUES (200,'STA',1,1,145);
INSERT INTO score VALUES (199,'krika40',5,16,1345);
INSERT INTO score VALUES (198,'puff85',9,34,4008);
INSERT INTO score VALUES (197,'Dem',0,2,162);
INSERT INTO score VALUES (196,'scorpion2T',0,8,395);
INSERT INTO score VALUES (195,'Trixy',3,4,500);
INSERT INTO score VALUES (194,'-=EVIL_DEAD=-',0,5,42);
INSERT INTO score VALUES (193,'Vasilisk',0,2,0);
INSERT INTO score VALUES (192,'FoxikV',3,24,1329);
INSERT INTO score VALUES (191,'Steel_Biker',11,22,2357);
INSERT INTO score VALUES (190,'elf_kz',4,20,1683);
INSERT INTO score VALUES (189,'999170',4,2,104);
INSERT INTO score VALUES (188,'HOMIK',6,34,1574);
INSERT INTO score VALUES (187,'Barby',0,3,206);
INSERT INTO score VALUES (186,'Deva',0,1,0);
INSERT INTO score VALUES (185,'Bigmother',7,23,2198);
INSERT INTO score VALUES (184,'Max_Suhov',0,1,109);
INSERT INTO score VALUES (183,'zlanzys',15,48,4120);
INSERT INTO score VALUES (182,'71948',1,7,155);
INSERT INTO score VALUES (181,'_DarkAngel_',16,39,4797);
INSERT INTO score VALUES (180,'DSK',1,5,138);
INSERT INTO score VALUES (179,'Leonid_22',0,2,35);
INSERT INTO score VALUES (178,'mansch',0,9,213);
INSERT INTO score VALUES (177,'212844',13,11,3164);
INSERT INTO score VALUES (176,'zolushka',9,8,1699);
INSERT INTO score VALUES (175,'Revolve',4,5,1222);
INSERT INTO score VALUES (174,'klipok_yo',10,20,1260);
INSERT INTO score VALUES (173,'Nelly2',2,10,552);
INSERT INTO score VALUES (172,'Nata',20,43,2704);
INSERT INTO score VALUES (171,'Lasty',10,14,1653);
INSERT INTO score VALUES (170,'Kill95',3,3,601);
INSERT INTO score VALUES (169,'zoran',2,2,355);
INSERT INTO score VALUES (168,'Monarh',0,2,341);
INSERT INTO score VALUES (167,'Exzersis',3,4,375);
INSERT INTO score VALUES (166,'kukl2003',1,1,152);
INSERT INTO score VALUES (165,'123456789101112f',20,59,5667);
INSERT INTO score VALUES (164,'LEO4',30,58,7500);
INSERT INTO score VALUES (163,'LEGIONER_2',1,7,189);
INSERT INTO score VALUES (162,'worldpc',5,14,1365);
INSERT INTO score VALUES (161,'KIBorg',6,8,606);
INSERT INTO score VALUES (160,'jooi',10,36,2018);
INSERT INTO score VALUES (159,'scooby',0,1,14);
INSERT INTO score VALUES (158,'rpetr',6,9,1150);
INSERT INTO score VALUES (157,'Lenyn',17,43,3791);
INSERT INTO score VALUES (156,'nifer',6,7,850);
INSERT INTO score VALUES (155,'_q_',7,6,746);
INSERT INTO score VALUES (154,'ERAW',0,2,53);
INSERT INTO score VALUES (153,'Bad_girl',8,18,1303);
INSERT INTO score VALUES (152,'vini-terminator',32,61,5862);
INSERT INTO score VALUES (151,'qwertvg',13,31,2198);
INSERT INTO score VALUES (150,'Evgeniys',0,2,68);
INSERT INTO score VALUES (149,'hirasima',1,4,115);
INSERT INTO score VALUES (148,'VladimirInnoc',11,11,1216);
INSERT INTO score VALUES (147,'Alatiel',10,18,1381);
INSERT INTO score VALUES (146,'bev083',64,134,14013);
INSERT INTO score VALUES (145,'Vega',7,7,1172);
INSERT INTO score VALUES (144,'RedHot',1,15,422);
INSERT INTO score VALUES (143,'HomerS',12,26,2258);
INSERT INTO score VALUES (142,'tumakov',15,24,2529);
INSERT INTO score VALUES (141,'MARK',19,38,4948);
INSERT INTO score VALUES (140,'ForDNSV',59,71,8688);
INSERT INTO score VALUES (139,'Katerina__T',6,15,1770);
INSERT INTO score VALUES (138,'tlm',8,29,1795);
INSERT INTO score VALUES (137,'X',12,17,2834);
INSERT INTO score VALUES (136,'frekf',43,63,5853);
INSERT INTO score VALUES (135,'TyMPuK',0,1,38);
INSERT INTO score VALUES (134,'drus4u',15,26,4309);
INSERT INTO score VALUES (133,'Aleksey1984',116,151,18705);
INSERT INTO score VALUES (132,'RJVKTDF',30,42,6284);
INSERT INTO score VALUES (131,'Devchonka',0,3,113);
INSERT INTO score VALUES (130,'peps00',4,4,817);
INSERT INTO score VALUES (129,'oop',0,5,354);
INSERT INTO score VALUES (128,'JohnJ',84,143,11766);
INSERT INTO score VALUES (127,'Deflower',2,9,768);
INSERT INTO score VALUES (126,'stack37',9,17,2779);
INSERT INTO score VALUES (125,'DRdre',2,11,353);
INSERT INTO score VALUES (124,'danila_badrow',0,1,0);
INSERT INTO score VALUES (123,'Alaska',7,11,1611);
INSERT INTO score VALUES (122,'nastja',37,71,7213);
INSERT INTO score VALUES (121,'SveetCherry',2,10,226);
INSERT INTO score VALUES (120,'klima',16,42,3475);
INSERT INTO score VALUES (119,'Sivens',35,54,6442);
INSERT INTO score VALUES (118,'VVEBER',8,13,1704);
INSERT INTO score VALUES (117,'toli',0,1,10);
INSERT INTO score VALUES (116,'zizzou',0,5,184);
INSERT INTO score VALUES (115,'5417',14,22,1009);
INSERT INTO score VALUES (114,'nezsveta',5,5,797);
INSERT INTO score VALUES (113,'Zig',6,29,2006);
INSERT INTO score VALUES (112,'spirit_of_war',2,5,624);
INSERT INTO score VALUES (111,'liness',9,21,1544);
INSERT INTO score VALUES (110,'e1',0,1,10);
INSERT INTO score VALUES (109,'Ya-ha',12,48,3206);
INSERT INTO score VALUES (108,'TymonArt',19,64,5201);
INSERT INTO score VALUES (107,'Stoneman',6,23,1949);
INSERT INTO score VALUES (106,'zuko',2,13,775);
INSERT INTO score VALUES (105,'LoVVVeee',19,22,4569);
INSERT INTO score VALUES (104,'Salli',15,31,3167);
INSERT INTO score VALUES (103,'Teran_5',5,18,2019);
INSERT INTO score VALUES (102,'Slawik',20,75,6584);
INSERT INTO score VALUES (101,'angelocheck',17,37,3628);
INSERT INTO score VALUES (100,'mbnvc',2,7,410);
INSERT INTO score VALUES (99,'Alex_W',5,10,686);
INSERT INTO score VALUES (98,'3453432453432',0,16,768);
INSERT INTO score VALUES (97,'kryz',6,37,1571);
INSERT INTO score VALUES (96,'freSHnes',20,44,4978);
INSERT INTO score VALUES (95,'tonic',4,8,710);
INSERT INTO score VALUES (94,'PGsukaochkastaya',33,101,9516);
INSERT INTO score VALUES (93,'PP',40,69,9099);
INSERT INTO score VALUES (92,'scorpion2R',2,5,420);
INSERT INTO score VALUES (91,'2389',0,5,351);
INSERT INTO score VALUES (90,'MONAKO',3,5,562);
INSERT INTO score VALUES (89,'FREDY',12,45,3961);
INSERT INTO score VALUES (88,'LLGG',31,81,9020);
INSERT INTO score VALUES (87,'Legend',11,16,1394);
INSERT INTO score VALUES (86,'1587',3,7,625);
INSERT INTO score VALUES (85,'FRIONIK',2,18,984);
INSERT INTO score VALUES (84,'_Starper_',15,51,2987);
INSERT INTO score VALUES (83,'Kompas_ZemNoi',5,18,1454);
INSERT INTO score VALUES (82,'1',0,1,106);
INSERT INTO score VALUES (81,'ktyecz',21,57,5979);
INSERT INTO score VALUES (80,'optikom',29,81,6235);
INSERT INTO score VALUES (79,'NaKeD',5,22,1674);
INSERT INTO score VALUES (78,'dingo',2,10,759);
INSERT INTO score VALUES (77,'Shaltay',0,1,59);
INSERT INTO score VALUES (76,'_RaVe_',36,91,6031);
INSERT INTO score VALUES (75,'holy',7,32,2538);
INSERT INTO score VALUES (74,'gaf',2,3,409);
INSERT INTO score VALUES (73,'vbif',1,1,50);
INSERT INTO score VALUES (72,'CoolDude',1,3,476);
INSERT INTO score VALUES (71,'=www=',8,16,839);
INSERT INTO score VALUES (70,'0919981887176515',1,13,1133);
INSERT INTO score VALUES (69,'rooboot',11,31,2435);
INSERT INTO score VALUES (68,'tupolev',12,33,4325);
INSERT INTO score VALUES (67,'_JaX_',0,1,12);
INSERT INTO score VALUES (66,'vitfel',22,69,6847);
INSERT INTO score VALUES (65,'fyrf',12,30,3167);
INSERT INTO score VALUES (64,'Fan9',13,39,2218);
INSERT INTO score VALUES (63,'tannya',30,56,7321);
INSERT INTO score VALUES (62,'AnutaAnuta',12,35,4070);
INSERT INTO score VALUES (61,'WERTY--',1,3,448);
INSERT INTO score VALUES (60,'Arsenal',32,52,6800);
INSERT INTO score VALUES (59,'SoD',13,30,2515);
INSERT INTO score VALUES (58,'rondo1986',1,2,177);
INSERT INTO score VALUES (57,'ALT*',10,20,2641);
INSERT INTO score VALUES (56,'Sha',39,61,5909);
INSERT INTO score VALUES (55,'PENESSANS',2,10,411);
INSERT INTO score VALUES (54,'Frifoll',1,2,401);
INSERT INTO score VALUES (53,'VEDMARKA',3,12,345);
INSERT INTO score VALUES (52,'KEPER',4,20,1468);
INSERT INTO score VALUES (51,'77777yo',7,23,2051);
INSERT INTO score VALUES (50,'772',1,8,611);
INSERT INTO score VALUES (49,'Alog1',20,40,6336);
INSERT INTO score VALUES (48,'AlekcandR_2',24,46,5799);
INSERT INTO score VALUES (47,'cruezer',17,49,3468);
INSERT INTO score VALUES (46,'89048553254',7,33,2946);
INSERT INTO score VALUES (45,'gigir',17,81,5626);
INSERT INTO score VALUES (44,'NATIX',2,11,440);
INSERT INTO score VALUES (43,'Gperop',0,2,36);
INSERT INTO score VALUES (42,'acrobat',10,34,1714);
INSERT INTO score VALUES (41,'EBrEH',5,11,1145);
INSERT INTO score VALUES (40,'combinator',14,33,3439);
INSERT INTO score VALUES (39,'PGmatv',74,98,11508);
INSERT INTO score VALUES (38,'DRON888',4,12,1354);
INSERT INTO score VALUES (37,'mf',4,12,737);
INSERT INTO score VALUES (36,'awr',0,1,0);
INSERT INTO score VALUES (35,'Galsi',26,48,7254);
INSERT INTO score VALUES (34,'Ytkin_Sasha',0,1,0);
INSERT INTO score VALUES (33,'OZA',5,9,904);
INSERT INTO score VALUES (32,'Svetik',22,41,4623);
INSERT INTO score VALUES (31,'combinatorrr',30,43,6790);
INSERT INTO score VALUES (30,'NeO',0,1,0);
INSERT INTO score VALUES (29,'-omax-',56,107,11504);
INSERT INTO score VALUES (28,'Dimkrut',47,38,7177);
INSERT INTO score VALUES (27,'AndreyS',5,12,1077);
INSERT INTO score VALUES (26,'165',0,1,49);
INSERT INTO score VALUES (25,'tiasur',1,4,283);
INSERT INTO score VALUES (24,'Exstaz',6,12,1506);
INSERT INTO score VALUES (23,'epytafy',1,4,177);
INSERT INTO score VALUES (22,'=lakmus=',17,37,4724);
INSERT INTO score VALUES (21,'STARS',20,30,4767);
INSERT INTO score VALUES (20,'Cap',7,15,1637);
INSERT INTO score VALUES (19,'VOROB',51,113,10710);
INSERT INTO score VALUES (18,'Trusnno',4,19,1767);
INSERT INTO score VALUES (17,'sk8er',5,17,1093);
INSERT INTO score VALUES (16,'Dst',49,126,14333);
INSERT INTO score VALUES (15,'semen',16,28,3070);
INSERT INTO score VALUES (14,'ol-li-li',5,29,2092);
INSERT INTO score VALUES (13,'lohhly',9,47,2512);
INSERT INTO score VALUES (12,'Morgik',21,24,4110);
INSERT INTO score VALUES (11,'VES',1,12,432);
INSERT INTO score VALUES (10,'Umu',68,64,9196);
INSERT INTO score VALUES (9,'newuser2003',4,16,1628);
INSERT INTO score VALUES (8,'PGzaporozes',0,8,421);
INSERT INTO score VALUES (7,'prick',16,14,3447);
INSERT INTO score VALUES (6,'gipon',72,105,15140);
INSERT INTO score VALUES (5,'rvd',3,39,1963);
INSERT INTO score VALUES (4,'maina',50,118,12573);
INSERT INTO score VALUES (3,'Bosya',1,3,30);
INSERT INTO score VALUES (2,'777',3,14,897);
INSERT INTO score VALUES (1,'065',96,140,15487);

