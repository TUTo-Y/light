#include "light.h"

int main(int argc, char *argv[])
{
    // 默认最大亮度
    int max_light = 0;
    // 默认当前亮度
    int light = 0;

    // 使用root权限执行
    if (getuid() != 0)
    {
        char path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
        if (len == -1)
        {
            fprintf(stderr, BRED("readlink 失败\n"));
            return -1;
        }
        path[len] = '\0';

        char *args[argc + 2];
        args[0] = "sudo";
        args[1] = path; // 使用绝对路径
        for (int i = 1; i < argc; i++)
        {
            args[i + 1] = argv[i];
        }
        args[argc + 1] = NULL;

        execvp("sudo", args); // 使用sudo执行程序

        fprintf(stderr, BRED("执行失败!\n"));
        return -1;
    }

    // 获取当前亮度信息
    printf(BBLU("最大亮度值: %d\n"), max_light = get_num_from_file(PATH "/max_brightness"));
    printf(BBLU("当前亮度值: %d\n"), light = get_num_from_file(PATH "/brightness"));

    if (light == -1 || max_light == -1)
    {
        fprintf(stderr, BRED("读取亮度值失败!\n"));
        return -1;
    }

    // 仅仅是查看亮度值
    if (argc == 1)
        return 0;

    // 获取将要设置的亮度值
    char *endptr;
    double num = strtod(argv[1], &endptr);

    // 检查将要设置的亮度值是否有效
    if (*endptr != '\0' || num < 0 || num > max_light)
    {
        fprintf(stderr, BRED("无效亮度值!\n"));
        return -1;
    }

    // 计算出实际亮度值
    int num_int = (int)(num <= 1.0 ? num * (double)max_light : num);

    // 检查当前亮度信息是否需要修改
    if (num_int == light)
    {
        printf(BYEL("当前亮度值已经是 %d\n"), num_int);
        return 0;
    }

    // 设置亮度值
    if (set_num_to_file(PATH "/brightness", num_int))
    {
        fprintf(stderr, BRED("设置亮度值失败!\n"));
        return -1;
    }

    // 检查当前亮度信息 (可省略)
    if (num_int != get_num_from_file(PATH "/brightness"))
    {
        fprintf(stderr, BRED("设置亮度值失败!\n"));
        return -1;
    }

    printf(BGRN("设置亮度值成功!\n"));
    printf(BGRN("当前亮度值 : %d\n"), num_int);
    return 0;
}