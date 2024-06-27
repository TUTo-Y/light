#include "light.h"

int get_num_from_file(const char *path)
{
    int ret = 0;
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        fprintf(stderr, BRED("打开 %s 失败!\n"), path);
        return -1;
    }
    fscanf(fp, "%d", &ret);
    fclose(fp);

    return ret;
}

int set_num_to_file(const char *path, int num)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        fprintf(stderr, BRED("打开 %s 失败!\n"), path);
        return -1;
    }
    fprintf(fp, "%d", num);
    fclose(fp);

    return 0;
}
