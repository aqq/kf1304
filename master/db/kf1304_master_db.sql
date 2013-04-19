/*
Navicat MySQL Data Transfer

Source Server         : 192.168.60.22_3306
Source Server Version : 50168
Source Host           : 192.168.60.22:3306
Source Database       : kf1304_master_db

Target Server Type    : MYSQL
Target Server Version : 50168
File Encoding         : 65001

Date: 2013-04-19 09:27:04
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `site_tb`
-- ----------------------------
DROP TABLE IF EXISTS `site_tb`;
CREATE TABLE `site_tb` (
  `site_id` varchar(255) NOT NULL,
  PRIMARY KEY (`site_id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of site_tb
-- ----------------------------

-- ----------------------------
-- Table structure for `test`
-- ----------------------------
DROP TABLE IF EXISTS `test`;
CREATE TABLE `test` (
  `T1` int(11) NOT NULL,
  `T2` int(11) DEFAULT NULL,
  PRIMARY KEY (`T1`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of test
-- ----------------------------
INSERT INTO `test` VALUES ('1', '1');
INSERT INTO `test` VALUES ('2', '3');

-- ----------------------------
-- Table structure for `worker_site_tb`
-- ----------------------------
DROP TABLE IF EXISTS `worker_site_tb`;
CREATE TABLE `worker_site_tb` (
  `ss_Id` int(11) NOT NULL AUTO_INCREMENT,
  `slave_id` int(11) NOT NULL,
  `site_id` varchar(255) NOT NULL,
  `site_task_sucess` int(11) DEFAULT '0',
  `site_task_failed` int(11) DEFAULT '0',
  PRIMARY KEY (`ss_Id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of worker_site_tb
-- ----------------------------
INSERT INTO `worker_site_tb` VALUES ('1', '1', '1688_aa', '8', '0');
INSERT INTO `worker_site_tb` VALUES ('2', '1', '114chn_1', '701', '0');
INSERT INTO `worker_site_tb` VALUES ('3', '2', '114chn_1', '151', '0');
INSERT INTO `worker_site_tb` VALUES ('4', '2', '1688_aa', '0', '0');
INSERT INTO `worker_site_tb` VALUES ('5', '2', 'hc360_aa', '0', '0');
INSERT INTO `worker_site_tb` VALUES ('6', '1', 'hc360_aa', '0', '0');

-- ----------------------------
-- Table structure for `worker_tb`
-- ----------------------------
DROP TABLE IF EXISTS `worker_tb`;
CREATE TABLE `worker_tb` (
  `slave_id` int(11) NOT NULL,
  `last_request_time` datetime DEFAULT NULL,
  `last_ip` varchar(255) DEFAULT NULL,
  `available_disk_space` float DEFAULT NULL,
  PRIMARY KEY (`slave_id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of worker_tb
-- ----------------------------
INSERT INTO `worker_tb` VALUES ('1', '2013-04-18 21:19:22', '10.2.112.95', '3.77051');
INSERT INTO `worker_tb` VALUES ('2', '2013-04-18 20:46:58', '10.2.112.92', '3.8877');
INSERT INTO `worker_tb` VALUES ('3', '2013-04-18 19:54:01', '10.2.112.92', '3.89062');
INSERT INTO `worker_tb` VALUES ('4', null, null, null);
INSERT INTO `worker_tb` VALUES ('5', null, null, null);
