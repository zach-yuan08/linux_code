#include <stdio.h>
#include <ncurses.h>  // ncurses库头文件
#include <string.h>
// 菜单选项
const char *menu_items[] = {
    "开始游戏",
    "设置",
    "关于",
    "退出",
    NULL  // 结束标志
};

int main() {
    int ch;               // 存储输入的字符
    int choice = 0;       // 当前选中的菜单项索引
    int highlight = 0;    // 高亮显示的菜单项索引

    // 初始化ncurses
    initscr();            // 初始化屏幕
    clear();              // 清空屏幕
    noecho();             // 关闭输入回显（不显示输入的字符）
    cbreak();             // 关闭行缓冲（直接读取单个字符）
    curs_set(0);          // 隐藏光标

    // 检查终端是否支持颜色
    if (has_colors() == FALSE) {
        endwin();
        printf("你的终端不支持颜色！\n");
        return 1;
    }
    start_color();        // 启用颜色功能
    init_pair(1, COLOR_GREEN, COLOR_BLACK);  // 定义颜色对1：绿色文字，黑色背景
    init_pair(2, COLOR_WHITE, COLOR_BLUE);   // 定义颜色对2：白色文字，蓝色背景

    // 主循环
    while (1) {
        clear();  // 清空屏幕
        int row, col;
        getmaxyx(stdscr, row, col);  // 获取终端窗口尺寸

        // 打印标题（居中显示）
        attron(COLOR_PAIR(1) | A_BOLD);  // 启用颜色对1和粗体
        mvprintw(row/4, (col - 16)/2, "=== 菜单演示程序 ===");
        attroff(COLOR_PAIR(1) | A_BOLD);  // 关闭属性

        // 打印菜单项
        for (int i = 0; menu_items[i] != NULL; i++) {
            // 计算菜单位置（居中显示）
            int x = (col - strlen(menu_items[i])) / 2;
            int y = row/2 + i;

            if (i == highlight) {  // 高亮显示当前选中项
                attron(COLOR_PAIR(2) | A_REVERSE);  // 启用颜色对2和反显
                mvprintw(y, x, "%s", menu_items[i]);
                attroff(COLOR_PAIR(2) | A_REVERSE);
            } else {  // 普通显示
                mvprintw(y, x, "%s", menu_items[i]);
            }
        }

        // 打印操作提示
        mvprintw(row - 4, (col - 30)/2, "使用 上下箭头 选择，按 Enter 确认");
        refresh();  // 刷新屏幕显示

        // 获取用户输入
        ch = getch();
        switch (ch) {
            case KEY_UP:    // 上箭头
                highlight--;
                if (highlight < 0)
                    highlight = 0;  // 限制最小索引
                break;
            case KEY_DOWN:  // 下箭头
                if (menu_items[highlight + 1] != NULL)
                    highlight++;    // 限制最大索引
                break;
            case 10:        // Enter键（ASCII码10）
                choice = highlight;
                break;
            case 27:        // ESC键（ASCII码27）
                endwin();   // 退出ncurses模式
                return 0;
            default:
                break;
        }

        // 处理用户选择
        if (ch == 10) {
            clear();
            mvprintw(row/2, (col - 20)/2, "你选择了: %s", menu_items[choice]);
            mvprintw(row/2 + 2, (col - 20)/2, "按任意键返回菜单...");
            refresh();
            getch();  // 等待用户按键

            if (choice == 3) {  // 选择"退出"
                break;
            }
        }
    }

    // 退出ncurses模式，恢复终端设置
    endwin();
    return 0;
}
