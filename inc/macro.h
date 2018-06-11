#ifndef MACRO_H_
#define MACRO_H_

#define MAX_USERNAME	64
#define MAX_PWD			64
#define MAX_IP			16

#define CTLP									"CTLP"	 // 管理平台类型
#define PBCS									"PBCS"	 // 交易C端类型

#define RECONNEC_INTERVAL_TIME					3		 // 重新登录的间隔时间

#define TS_FUNC_QUOTE_LOGIN						"000001" // 行情登录
#define TS_FUNC_QUOTE_SUB						"000002" // 行情订阅
#define TS_FUNC_QUOTE_PUSH						"000003" // 行情推送
#define TS_FUNC_CANCEL_QUOTE_SUB				"000004" // 取消订阅

#define TS_RES_CODE_SUCCESS                     "000000" // 操作成功
#define TS_RES_CODE_QUOTE_LOGIN_FAILED			"000002" // 行情登录失败
#define TS_RES_CODE_QUOTE_SUB_FAILED			"000003" // 行情订阅失败

#define HEARTBEAT_INTERVAL						3		 // 心跳包的间隔时间


#endif