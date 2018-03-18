-- 以utf8编码格式创建创建数据库
create database if not exists discern_img default charset utf8 collate utf8_general_ci;

-- 选择数据库
use discern_img;

-- 创建 人物表
DROP TABLE IF EXISTS `face`;
CREATE TABLE `face`(
    `id`            int(4)  UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `age`           int(4),
    `beauty`        double(20,5),
    `gender`        varchar(32),
    `glass`         int(4),
    `race`          varchar(32),
    `url_img`       varchar(128),
    `time`          timestamp NOT NULL ON UPDATE CURRENT_TIMESTAMP DEFAULT CURRENT_TIMESTAMP
)DEFAULT CHARSET=utf8;

-- 创建 菜品表
DROP TABLE IF EXISTS `food`;
CREATE TABLE `food`(
    `id`            int(4)  UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `calorie`       varchar(32),
    `has_calorie`   int(4),
    `name`          varchar(32),
    `possible`      varchar(32),
    `url_img`       varchar(128),
    `time`          timestamp NOT NULL ON UPDATE CURRENT_TIMESTAMP DEFAULT CURRENT_TIMESTAMP
)DEFAULT CHARSET=utf8;

-- 创建 动物表
DROP TABLE IF EXISTS `animal`;
CREATE TABLE `animal`(
    `id`            int(4)  UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `name`          varchar(32),
    `score`         varchar(32),
    `url_img`       varchar(128),
    `time`          timestamp NOT NULL ON UPDATE CURRENT_TIMESTAMP DEFAULT CURRENT_TIMESTAMP
)DEFAULT CHARSET=utf8;

-- 创建 植物表
DROP TABLE IF EXISTS `botany`;
CREATE TABLE `botany`(
    `id`            int(4)  UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `name`          varchar(32),
    `score`         double(20,5),
    `url_img`       varchar(128),
    `time`          timestamp NOT NULL ON UPDATE CURRENT_TIMESTAMP DEFAULT CURRENT_TIMESTAMP
)DEFAULT CHARSET=utf8;
