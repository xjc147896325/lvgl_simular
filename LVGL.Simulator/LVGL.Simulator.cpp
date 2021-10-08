/*
 * PROJECT:   LVGL PC Simulator using Visual Studio
 * FILE:      LVGL.Simulator.cpp
 * PURPOSE:   Implementation for LVGL ported to Windows Desktop
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include <Windows.h>

#include "resource.h"

#if _MSC_VER >= 1200
 // Disable compilation warnings.
#pragma warning(push)
// nonstandard extension used : bit field types other than int
#pragma warning(disable:4214)
// 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
#endif

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lv_demos/lv_demo.h"
#include "lv_drivers/win32drv/win32drv.h"
#include "lv_lib_freetype/lv_freetype.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#if _MSC_VER >= 1200
// Restore compilation warnings.
#pragma warning(pop)
#endif

lv_fs_res_t lv_win32_filesystem_driver_error_from_win32(
    DWORD Error)
{
    lv_fs_res_t res;

    switch (Error)
    {
    case ERROR_SUCCESS:
        res = LV_FS_RES_OK;
        break;
    case ERROR_BAD_UNIT:
    case ERROR_NOT_READY:
    case ERROR_CRC:
    case ERROR_SEEK:
    case ERROR_NOT_DOS_DISK:
    case ERROR_WRITE_FAULT:
    case ERROR_READ_FAULT:
    case ERROR_GEN_FAILURE:
    case ERROR_WRONG_DISK:
        res = LV_FS_RES_HW_ERR;
        break;
    case ERROR_INVALID_HANDLE:
    case ERROR_INVALID_TARGET_HANDLE:
        res = LV_FS_RES_FS_ERR;
        break;
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_DRIVE:
    case ERROR_NO_MORE_FILES:
    case ERROR_SECTOR_NOT_FOUND:
    case ERROR_BAD_NETPATH:
    case ERROR_BAD_NET_NAME:
    case ERROR_BAD_PATHNAME:
    case ERROR_FILENAME_EXCED_RANGE:
        res = LV_FS_RES_NOT_EX;
        break;
    case ERROR_DISK_FULL:
        res = LV_FS_RES_FULL;
        break;
    case ERROR_SHARING_VIOLATION:
    case ERROR_LOCK_VIOLATION:
    case ERROR_DRIVE_LOCKED:
        res = LV_FS_RES_LOCKED;
        break;
    case ERROR_ACCESS_DENIED:
    case ERROR_CURRENT_DIRECTORY:
    case ERROR_WRITE_PROTECT:
    case ERROR_NETWORK_ACCESS_DENIED:
    case ERROR_CANNOT_MAKE:
    case ERROR_FAIL_I24:
    case ERROR_SEEK_ON_DEVICE:
    case ERROR_NOT_LOCKED:
    case ERROR_LOCK_FAILED:
        res = LV_FS_RES_DENIED;
        break;
    case ERROR_BUSY:
        res = LV_FS_RES_BUSY;
        break;
    case ERROR_TIMEOUT:
        res = LV_FS_RES_TOUT;
        break;
    case ERROR_NOT_SAME_DEVICE:
    case ERROR_DIRECT_ACCESS_HANDLE:
        res = LV_FS_RES_NOT_IMP;
        break;
    case ERROR_TOO_MANY_OPEN_FILES:
    case ERROR_ARENA_TRASHED:
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_INVALID_BLOCK:
    case ERROR_OUT_OF_PAPER:
    case ERROR_SHARING_BUFFER_EXCEEDED:
    case ERROR_NOT_ENOUGH_QUOTA:
        res = LV_FS_RES_OUT_OF_MEM;
        break;
    case ERROR_INVALID_FUNCTION:
    case ERROR_INVALID_ACCESS:
    case ERROR_INVALID_DATA:
    case ERROR_BAD_COMMAND:
    case ERROR_BAD_LENGTH:
    case ERROR_INVALID_PARAMETER:
    case ERROR_NEGATIVE_SEEK:
        res = LV_FS_RES_INV_PARAM;
        break;
    default:
        res = LV_FS_RES_UNKNOWN;
        break;
    }

    return res;
}

//void* lv_win32_filesystem_driver_open_callback(
//    lv_fs_drv_t* drv,
//    const char* path,
//    lv_fs_mode_t mode)
//{
//    UNREFERENCED_PARAMETER(drv);
//
//
//
//}
//
//void fuck()
//{
//    static lv_fs_drv_t filesystem_driver;
//    lv_fs_drv_init(&filesystem_driver);
//
//    filesystem_driver.letter;
//    filesystem_driver.ready_cb;
//
//    filesystem_driver.open_cb;
//    filesystem_driver.close_cb;
//    filesystem_driver.read_cb;
//    filesystem_driver.write_cb;
//    filesystem_driver.seek_cb;
//    filesystem_driver.tell_cb;
//
//    filesystem_driver.dir_open_cb;
//    filesystem_driver.dir_read_cb;
//    filesystem_driver.dir_close_cb;
//
//    filesystem_driver.user_data;
//
//    lv_fs_drv_register(&filesystem_driver);
//}


/*********************
 *      DEFINES
 *********************/
 /* 大小 */
#define SIZE 4

/* 布局 */
#define LV_100ASK_2048_GAME_BOX_W			(260)
#define LV_100ASK_2048_GAME_BOX_H			(260)
#define LV_100ASK_2048_NUMBER_BOX_W			(55)
#define LV_100ASK_2048_NUMBER_BOX_H			(30)
#define LV_100ASK_2048_CURRENT_SCORE_W		(65)
#define LV_100ASK_2048_CURRENT_SCORE_H		(35)
#define LV_100ASK_2048_BEST_SCORE_W			(65)
#define LV_100ASK_2048_BEST_SCORE_H			(35)

/* 颜色 */
#define LV_100ASK_2048_GAME_BG_COLOR 		lv_color_hex(0xe8e5db) //LV_COLOR_MAKE(0xCD, 0xC1, 0xB4)
#define LV_100ASK_2048_GAME_BOX_COLOR 		lv_color_hex(0xBBADA0)
#define LV_100ASK_2048_NUMBER_BOX_COLOR 	lv_color_hex(0xCDC1B4)
#define LV_100ASK_2048_NUMBER_2_COLOR 		lv_color_hex(0xeee4da)
#define LV_100ASK_2048_NUMBER_4_COLOR 		lv_color_hex(0xeee1c9)
#define LV_100ASK_2048_NUMBER_8_COLOR 		lv_color_hex(0xf3b27a)
#define LV_100ASK_2048_NUMBER_16_COLOR 		lv_color_hex(0xf69664)
#define LV_100ASK_2048_NUMBER_32_COLOR 		lv_color_hex(0xf77c5f)
#define LV_100ASK_2048_NUMBER_64_COLOR 		lv_color_hex(0xf75f3b)
#define LV_100ASK_2048_NUMBER_128_COLOR 	lv_color_hex(0xedd073)
#define LV_100ASK_2048_NUMBER_256_COLOR 	lv_color_hex(0xEDCC61)
#define LV_100ASK_2048_NUMBER_512_COLOR 	lv_color_hex(0xEDCC61)
#define LV_100ASK_2048_NUMBER_1024_COLOR 	lv_color_hex(0xEDCC61)
#define LV_100ASK_2048_NUMBER_2048_COLOR 	lv_color_hex(0xEDC22E)


/**********************
 *      TYPEDEFS
 **********************/
typedef struct _lv_100ask_2048_game {
    lv_obj_t* bg;					/* 背景 */
    lv_obj_t* canvas_stage;		/* 游戏舞台 */
    lv_obj_t* label_best_score;	/* 展示最高分数 */
    lv_obj_t* label_current_score;	/* 展示当前分数 */
//    lv_task_t* task_handle;		/* 任务句柄 */

    uint8_t game_board[SIZE][SIZE];	/* 2048矩阵 */
    uint32_t current_score_value;	/* 当前分数 */
    bool play_game;					/* 游戏状态 */
} T_lv_100ask_2048_game, * PT_lv_100ask_2048_game;

/**********************
 *  STATIC VARIABLES

 **********************/
static PT_lv_100ask_2048_game g_pt_lv_100ask_2048_game;  // 数据结构体


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_100ask_game_2048_init(void);
static void lv_gamepad_2048_init(void);
static lv_obj_t* add_title(lv_obj_t* obj_space, const char* txt);
static void initBoard(lv_obj_t* parent, uint8_t board[SIZE][SIZE]);
static void initBoardNumber(uint8_t board[SIZE][SIZE]);
static void drawBoard(lv_obj_t* parent, char* number, \
    uint16_t canvas_w, uint16_t canvas_h, \
    uint16_t rect_dsc_x, uint16_t rect_dsc_y, \
    lv_color_t rgb32);

static void addRandom(uint8_t board[SIZE][SIZE]);
static lv_color_t Num2Color(uint32_t num);
static uint8_t findTarget(uint8_t array[SIZE], uint8_t x, uint8_t stop);
static bool slideArray(uint8_t array[SIZE]);
static void rotateBoard(uint8_t board[SIZE][SIZE]);
static bool moveUp(uint8_t board[SIZE][SIZE]);
static bool moveLeft(uint8_t board[SIZE][SIZE]);
static bool moveDown(uint8_t board[SIZE][SIZE]);
static bool moveRight(uint8_t board[SIZE][SIZE]);
static bool findPairDown(uint8_t board[SIZE][SIZE]);
static uint8_t countEmpty(uint8_t board[SIZE][SIZE]);
static bool gameEnded(uint8_t board[SIZE][SIZE]);
static char* Int2String(int num, char* str);

// 事件
static void event_handler_play_2048(lv_event_t * event);
static void event_handler_2048_game_up(lv_event_t* event);
static void event_handler_2048_game_down(lv_event_t* event);
static void event_handler_2048_game_left(lv_event_t* event);
static void event_handler_2048_game_right(lv_event_t* event);



void lv_100ask_game_2048(void)
{
    /* 申请内存 */
    g_pt_lv_100ask_2048_game = (T_lv_100ask_2048_game*)malloc(sizeof(T_lv_100ask_2048_game));

    /* 初始化桌面背景 */
    g_pt_lv_100ask_2048_game->bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(g_pt_lv_100ask_2048_game->bg, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_y(g_pt_lv_100ask_2048_game->bg, 0);

    /* 初始化游戏舞台 */
    g_pt_lv_100ask_2048_game->canvas_stage = lv_obj_create(g_pt_lv_100ask_2048_game->bg);
    lv_obj_set_style_radius(g_pt_lv_100ask_2048_game->canvas_stage, LV_STATE_DEFAULT, 0); // 设置圆角
    lv_obj_set_style_bg_color(g_pt_lv_100ask_2048_game->canvas_stage, LV_100ASK_2048_GAME_BOX_COLOR, LV_STATE_DEFAULT); //设置颜色
    lv_obj_set_size(g_pt_lv_100ask_2048_game->canvas_stage, LV_100ASK_2048_GAME_BOX_W, LV_100ASK_2048_GAME_BOX_H);
    lv_obj_align(g_pt_lv_100ask_2048_game->canvas_stage, LV_ALIGN_TOP_MID, 0, LV_100ASK_2048_NUMBER_BOX_H);

    /* 初始化主界面 */
    g_pt_lv_100ask_2048_game->play_game = true;
    g_pt_lv_100ask_2048_game->current_score_value = 0;
    lv_100ask_game_2048_init();


    ///* 分配屏幕触摸事件处理 */
//    lv_obj_add_event_cb(lv_layer_top(), event_handler_play_2048, LV_EVENT_CLICKED, NULL);

    lv_gamepad_2048_init();


    /* 创建app标题 */
    add_title(g_pt_lv_100ask_2048_game->bg, "2048 GAME");

}


// 添加标题
static lv_obj_t* add_title(lv_obj_t* obj_space, const char* txt)
{
    lv_color_t c;
    c.ch.red = 0x00;
    c.ch.green = 0xff;
    c.ch.blue = 0x00;
    c.full = lv_color_to32(c);

    // 添加标题
    lv_obj_t* title = lv_label_create(obj_space);
    lv_obj_set_style_text_color(title, c, LV_STATE_PRESSED);  //bug!!!!!!!!!!!!!
    lv_obj_set_style_text_font(title, &lv_font_montserrat_32, LV_STATE_DEFAULT);
    lv_label_set_text(title, txt);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0,
        0);
    return title;
}


static void lv_100ask_game_2048_init(void)
{
    /* 游戏说明 */
    lv_obj_t* game_title = lv_label_create(g_pt_lv_100ask_2048_game->bg);
    lv_obj_set_style_text_font(game_title, &lv_font_montserrat_32, LV_STATE_DEFAULT);             // 设置文字大小
    lv_label_set_recolor(game_title, true); 					 /*Enable re-coloring by commands in the text*/
    lv_obj_set_align(game_title, LV_ALIGN_LEFT_MID);		 /*Center aligned lines*/
    lv_obj_align_to(game_title, g_pt_lv_100ask_2048_game->canvas_stage, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(game_title, "#776e65 2048# ");

    lv_obj_t* game_tutorial = lv_label_create(g_pt_lv_100ask_2048_game->bg);
    lv_obj_set_align(game_tutorial, LV_ALIGN_OUT_TOP_LEFT);		 /*Center aligned lines*/
    lv_obj_align_to(game_tutorial, game_title, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_label_set_text(game_tutorial, "Join the SB, \nget to 2048!\n");
    //lv_label_set_text(game_tutorial, "合并色块，得出更大的数字！");
    lv_obj_set_width(game_tutorial, 200);

//   /* LOGO */
//   LV_IMG_DECLARE(img_lv_100ask_demo_logo);
//   lv_obj_t* logo = lv_img_create(g_pt_lv_100ask_2048_game->bg);
//   lv_img_set_src(logo, &img_lv_100ask_demo_logo);
//   lv_obj_align(logo, game_tutorial, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);  //(lv_obj_get_width(logo)/4)

   /* 公司信息 */
   lv_obj_t* label_logo = lv_label_create(g_pt_lv_100ask_2048_game->bg);
   lv_obj_set_style_text_font(label_logo, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
   lv_label_set_text(label_logo, "I'm your dad!");
   lv_obj_align_to(label_logo, game_tutorial, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

   /* 最高记录显示区域 */
   lv_obj_t* obj_best_source = lv_obj_create(g_pt_lv_100ask_2048_game->bg);
   lv_obj_set_size(obj_best_source, 80, 65);
   lv_obj_align_to(obj_best_source, g_pt_lv_100ask_2048_game->canvas_stage, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);

   /* 最高记录标题 */
   lv_obj_t* best_source_tip_text = lv_label_create(obj_best_source); /* 创建标签 */
   lv_obj_set_style_text_font(best_source_tip_text, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
   lv_label_set_text(best_source_tip_text, "BEST");
   lv_obj_align(best_source_tip_text, LV_ALIGN_TOP_MID, 0, 0);  /* 居中 */

   /* 最高记录分数 */
   g_pt_lv_100ask_2048_game->label_best_score = lv_label_create(obj_best_source); /* 创建标签 */
   lv_obj_set_style_text_font(g_pt_lv_100ask_2048_game->label_best_score, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
   lv_label_set_text(g_pt_lv_100ask_2048_game->label_best_score, "4096");
   lv_obj_align(g_pt_lv_100ask_2048_game->label_best_score, LV_ALIGN_BOTTOM_MID, 0, 0);  /* 居中 */

   /* 当前分数显示区域 */
   lv_obj_t* obj_current_source = lv_obj_create(g_pt_lv_100ask_2048_game->bg);
   lv_obj_set_size(obj_current_source, 80, 65);
   lv_obj_align_to(obj_current_source, obj_best_source, LV_ALIGN_OUT_BOTTOM_MID, 0, (lv_obj_get_height(obj_best_source) / 5));

   /* 当前分提示 */
   lv_obj_t* current_source_tip_text = lv_label_create(obj_current_source); /* 创建标签 */
   lv_obj_set_style_text_font(current_source_tip_text, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
   lv_label_set_text(current_source_tip_text, "SORCE");
   lv_obj_align(current_source_tip_text, LV_ALIGN_TOP_MID, 0, 0);           /* 居中 */

   /* 当前分数 */
   g_pt_lv_100ask_2048_game->label_current_score = lv_label_create(obj_current_source);         /* 创建标签 */
   lv_obj_set_style_text_font(g_pt_lv_100ask_2048_game->label_current_score, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
   lv_label_set_text(g_pt_lv_100ask_2048_game->label_current_score, "0");
   lv_obj_align(g_pt_lv_100ask_2048_game->label_current_score, LV_ALIGN_BOTTOM_MID, 0, 0);   /* 当前画布居中 */

   /* 初始化游戏数字 */
   initBoardNumber(g_pt_lv_100ask_2048_game->game_board);

   /* 初始化游戏舞台中的每个方格块 */
   initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
}


static void lv_gamepad_2048_init(void) {

    /* up */
    lv_obj_t* btn_up = lv_btn_create(g_pt_lv_100ask_2048_game->bg); /* 创建btn */
    lv_obj_t* btn_up_text = lv_label_create(btn_up); /* 创建标签 */
    //lv_obj_set_style_text_font(btn_up_text, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
    lv_label_set_text(btn_up_text, "UP");
    lv_obj_align(btn_up_text, LV_ALIGN_TOP_MID, 0, 0);  /* 居中 */
    lv_obj_set_size(btn_up, 100, 40);
    lv_obj_set_align(btn_up, LV_ALIGN_LEFT_MID);
    
    lv_obj_add_event_cb(btn_up, event_handler_2048_game_up, LV_EVENT_CLICKED, NULL);

    /* left */
    lv_obj_t* btn_left = lv_btn_create(g_pt_lv_100ask_2048_game->bg); /* 创建btn */
    lv_obj_t* btn_left_text = lv_label_create(btn_left); /* 创建标签 */
    //lv_obj_set_style_text_font(btn_up_text, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
    lv_label_set_text(btn_left_text, "LEFT");
    lv_obj_align(btn_left_text, LV_ALIGN_TOP_MID, 0, 0);  /* 居中 */
    lv_obj_set_size(btn_left, 50, 40);
    lv_obj_align_to(btn_left, btn_up, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(btn_left, event_handler_2048_game_left, LV_EVENT_CLICKED, NULL);

    /* right */
    lv_obj_t* btn_right = lv_btn_create(g_pt_lv_100ask_2048_game->bg); /* 创建btn */
    lv_obj_t* btn_right_text = lv_label_create(btn_right); /* 创建标签 */
    //lv_obj_set_style_text_font(btn_up_text, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
    lv_label_set_text(btn_right_text, "RIGHT");
    lv_obj_align(btn_right_text, LV_ALIGN_TOP_MID, 0, 0);  /* 居中 */
    lv_obj_set_size(btn_right, 50, 40);
    lv_obj_align_to(btn_right, btn_up, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_event_cb(btn_right, event_handler_2048_game_right, LV_EVENT_CLICKED, NULL);

    /* down */
    lv_obj_t* btn_down = lv_btn_create(g_pt_lv_100ask_2048_game->bg); /* 创建btn */
    lv_obj_t* btn_down_text = lv_label_create(btn_down); /* 创建标签 */
    //lv_obj_set_style_text_font(btn_up_text, &lv_font_montserrat_10, LV_STATE_DEFAULT);             // 设置文字大小
    lv_label_set_text(btn_down_text, "DOWN");
    lv_obj_align(btn_down_text, LV_ALIGN_TOP_MID, 0, 0);  /* 居中 */
    lv_obj_set_size(btn_down, 100, 40);
    lv_obj_align_to(btn_down, btn_left, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(btn_down, event_handler_2048_game_down, LV_EVENT_CLICKED, NULL);



}





static void initBoard(lv_obj_t* parent, uint8_t board[SIZE][SIZE])
{
    /* 清除之前所有的子对象 */
    lv_obj_clean(parent);

    /* 当前分数 */
    char str_current_score[8] = { 0 };
    lv_label_set_text(g_pt_lv_100ask_2048_game->label_current_score, Int2String(g_pt_lv_100ask_2048_game->current_score_value, str_current_score));

    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            if (board[x][y] != 0) {
                //sprintf(tmp_char, "%d", board[x][y]);
                char tmp_char[8];
                snprintf(tmp_char, 8, "%u", (uint32_t)1 << board[x][y]);
                drawBoard(parent, tmp_char, \
                    LV_100ASK_2048_NUMBER_BOX_W, LV_100ASK_2048_NUMBER_BOX_H, \
                    (8 * (y + 1)) + (LV_100ASK_2048_NUMBER_BOX_W * (y + 1 - 1)), (8 * (x + 1)) + (LV_100ASK_2048_NUMBER_BOX_W * (x + 1 - 1)), Num2Color((uint32_t)1 << board[x][y]));
            }
            else {
                char block[1];
                block[0] = ' ';
                drawBoard(parent, block, \
                    LV_100ASK_2048_NUMBER_BOX_W, LV_100ASK_2048_NUMBER_BOX_H, \
                    (8 * (y + 1)) + (LV_100ASK_2048_NUMBER_BOX_W * (y + 1 - 1)), (8 * (x + 1)) + (LV_100ASK_2048_NUMBER_BOX_W * (x + 1 - 1)), LV_100ASK_2048_NUMBER_BOX_COLOR);
            }
        }
    }
}


static void drawBoard(lv_obj_t* parent, char* number, \
    uint16_t canvas_w, uint16_t canvas_h, \
    uint16_t rect_dsc_x, uint16_t rect_dsc_y, \
    lv_color_t rgb32)
{

    /* 创建方格块 */
    lv_obj_t* obj = lv_obj_create(parent);
    lv_obj_set_size(obj, canvas_w, canvas_h);
    lv_obj_set_style_bg_color(obj, rgb32, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_STATE_DEFAULT); // 设置圆角
    lv_obj_set_style_border_opa(obj, LV_OPA_0, LV_STATE_DEFAULT); // 边框透明度
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_STATE_DEFAULT); // 字体大小

    //lv_obj_set_event_cb(obj, event_handler_play_2048);  /* 分配事件处理 */
    //lv_obj_set_gesture_parent(obj, false);
    //lv_obj_set_click(obj, true);
    lv_obj_align(obj, LV_ALIGN_TOP_LEFT, rect_dsc_x, rect_dsc_y);

    /* 数字 */
    lv_obj_t* icon = lv_label_create(obj); /* 创建标签 */
    lv_label_set_text(icon, number);
    lv_obj_align_to(icon, NULL, LV_ALIGN_CENTER, 0, 0);  /* 当前画布居中 */

}



// 10进制
static char* Int2String(int num, char* str)
{
    int i = 0;//指示填充str
    if (num < 0)//如果num为负数，将num变正
    {
        num = -num;
        str[i++] = '-';
    }
    //转换
    do
    {
        str[i++] = num % 10 + 48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0'
        num /= 10;//去掉最低位
    } while (num);//num不为0继续循环

    str[i] = '\0';

    //确定开始调整的位置
    int j = 0;
    if (str[0] == '-')//如果有负号，负号不用调整
    {
        j = 1;//从第二位开始调整
        ++i;//由于有负号，所以交换的对称轴也要后移1位
    }
    //对称交换
    for (; j < i / 2; j++)
    {
        //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b;
        str[j] = str[j] + str[i - 1 - j];
        str[i - 1 - j] = str[j] - str[i - 1 - j];
        str[j] = str[j] - str[i - 1 - j];
    }

    return str;//返回转换后的值
}


static void initBoardNumber(uint8_t board[SIZE][SIZE]) {
    uint8_t x, y;
    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE; y++) {
            board[x][y] = 0;
        }
    }

    /* 初始化两个随机位置的随机数 */
    addRandom(board);
    addRandom(board);

    g_pt_lv_100ask_2048_game->current_score_value = 0;
}


static void addRandom(uint8_t board[SIZE][SIZE]) {
    static bool initialized = false;
    uint8_t x, y;
    uint8_t r, len = 0;
    uint8_t n, list[SIZE * SIZE][2];

    if (!initialized) {
        srand(time(NULL));
        initialized = true;
    }

    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE; y++) {
            if (board[x][y] == 0) {
                list[len][0] = x;
                list[len][1] = y;
                len++;
            }
        }
    }

    if (len > 0) {
        r = rand() % len;
        x = list[r][0];
        y = list[r][1];
        n = (rand() % 10) / 9 + 1;
        board[x][y] = n;
    }
}


static lv_color_t Num2Color(uint32_t num) {
    switch (num)
    {
    case 2:		return LV_100ASK_2048_NUMBER_2_COLOR;
    case 4:		return LV_100ASK_2048_NUMBER_4_COLOR;
    case 8:		return LV_100ASK_2048_NUMBER_8_COLOR;
    case 16:	return LV_100ASK_2048_NUMBER_16_COLOR;
    case 32:	return LV_100ASK_2048_NUMBER_32_COLOR;
    case 64:	return LV_100ASK_2048_NUMBER_64_COLOR;
    case 128:	return LV_100ASK_2048_NUMBER_128_COLOR;
    case 256:	return LV_100ASK_2048_NUMBER_256_COLOR;
    case 512:	return LV_100ASK_2048_NUMBER_512_COLOR;
    case 1024:	return LV_100ASK_2048_NUMBER_1024_COLOR;
    case 2048:	return LV_100ASK_2048_NUMBER_2048_COLOR;
    default:	return LV_100ASK_2048_NUMBER_2048_COLOR;
    }

}


static uint8_t findTarget(uint8_t array[SIZE], uint8_t x, uint8_t stop) {
    uint8_t t;
    // if the position is already on the first, don't evaluate
    if (x == 0) {
        return x;
    }
    for (t = (x - 1);; t--) {
        if (array[t] != 0) {
            if (array[t] != array[x]) {
                // merge is not possible, take next position
                return (t + 1);
            }
            return t;
        }
        else {
            // we should not slide further, return this one
            if (t == stop) {
                return t;
            }
        }
    }
    // we did not find a
    return x;
}

static bool slideArray(uint8_t array[SIZE]) {
    bool success = false;
    uint8_t x, t, stop = 0;

    for (x = 0; x < SIZE; x++) {
        if (array[x] != 0) {
            t = findTarget(array, x, stop);
            // if target is not original position, then move or merge
            if (t != x) {
                // if target is zero, this is a move
                if (array[t] == 0) {
                    array[t] = array[x];
                }
                else if (array[t] == array[x]) {
                    // merge (increase power of two)
                    array[t]++;
                    // increase score
                    g_pt_lv_100ask_2048_game->current_score_value += (uint32_t)1 << array[t];
                    // set stop to avoid double merge
                    stop = t + 1;
                }
                array[x] = 0;
                success = true;
            }
        }
    }
    return success;
}


static void rotateBoard(uint8_t board[SIZE][SIZE]) {
    uint8_t i, j, n = SIZE;
    uint8_t tmp;
    for (i = 0; i < n / 2; i++) {
        for (j = i; j < n - i - 1; j++) {
            tmp = board[i][j];
            board[i][j] = board[j][n - i - 1];
            board[j][n - i - 1] = board[n - i - 1][n - j - 1];
            board[n - i - 1][n - j - 1] = board[n - j - 1][i];
            board[n - j - 1][i] = tmp;
        }
    }
}


static bool moveUp(uint8_t board[SIZE][SIZE]) {
    bool success = false;
    uint8_t x;
    for (x = 0; x < SIZE; x++) {
        success |= slideArray(board[x]);
    }
    return success;
}


static bool moveLeft(uint8_t board[SIZE][SIZE]) {
    bool success;
    rotateBoard(board);
    success = moveUp(board);
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    return success;
}


static bool moveDown(uint8_t board[SIZE][SIZE]) {
    bool success;
    rotateBoard(board);
    rotateBoard(board);
    success = moveUp(board);
    rotateBoard(board);
    rotateBoard(board);
    return success;
}


static bool moveRight(uint8_t board[SIZE][SIZE]) {
    bool success;
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    success = moveUp(board);
    rotateBoard(board);
    return success;
}


static bool findPairDown(uint8_t board[SIZE][SIZE]) {
    bool success = false;
    uint8_t x, y;
    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE - 1; y++) {
            if (board[x][y] == board[x][y + 1]) return true;
        }
    }
    return success;
}


static uint8_t countEmpty(uint8_t board[SIZE][SIZE]) {
    uint8_t x, y;
    uint8_t count = 0;
    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE; y++) {
            if (board[x][y] == 0) {
                count++;
            }
        }
    }
    return count;
}


static bool gameEnded(uint8_t board[SIZE][SIZE]) {
    bool ended = true;
    if (countEmpty(board) > 0) return false;
    if (findPairDown(board)) return false;
    rotateBoard(board);
    if (findPairDown(board)) ended = false;
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    return ended;
}



/* 按键方式 */
static void event_handler_2048_game_left(lv_event_t* event)
{
    if (moveUp(g_pt_lv_100ask_2048_game->game_board))
    {
        initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
        addRandom(g_pt_lv_100ask_2048_game->game_board);
        initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
        if (gameEnded(g_pt_lv_100ask_2048_game->game_board)) {
            printf("         GAME OVER          \n");
        }
    }

}

static void event_handler_2048_game_right(lv_event_t* event)
{
    if (moveDown(g_pt_lv_100ask_2048_game->game_board))
    {
        initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
        addRandom(g_pt_lv_100ask_2048_game->game_board);
        initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
        if (gameEnded(g_pt_lv_100ask_2048_game->game_board)) {
            printf("         GAME OVER          \n");
        }
    }
}

static void event_handler_2048_game_up(lv_event_t* event)
{
    if (moveLeft(g_pt_lv_100ask_2048_game->game_board))
    {
        initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
        addRandom(g_pt_lv_100ask_2048_game->game_board);
        initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
        if (gameEnded(g_pt_lv_100ask_2048_game->game_board)) {
            printf("         GAME OVER          \n");
        }
    }
}

static void event_handler_2048_game_down(lv_event_t* event)
{
    if (moveRight(g_pt_lv_100ask_2048_game->game_board))
    {
        initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
        addRandom(g_pt_lv_100ask_2048_game->game_board);
        initBoard(g_pt_lv_100ask_2048_game->canvas_stage, g_pt_lv_100ask_2048_game->game_board);
        if (gameEnded(g_pt_lv_100ask_2048_game->game_board)) {
            printf("         GAME OVER          \n");
        }
    }
}


int main()
{
    lv_init();

    if (!lv_win32_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        800,
        480,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))
    {
        return -1;
    }

    lv_win32_add_all_input_devices_to_group(NULL);


    lv_100ask_game_2048();

    // ----------------------------------
    // Task handler loop
    // ----------------------------------

    while (!lv_win32_quit_signal)
    {

        lv_task_handler();
        Sleep(1);
    }

    return 0;
}
