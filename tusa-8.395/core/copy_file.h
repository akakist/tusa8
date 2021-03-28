#ifndef COPY_FILE_H
#define COPY_FILE_H
#include <sys/types.h>
#ifndef WIN32
#include <zlib.h>
#endif
#include <string>
#include <vector>
#include <map>


#ifdef WIN32
struct ms_stat {
    _dev_t st_dev;
    _ino_t st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    _dev_t st_rdev;
    _off_t st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
};

extern "C" {
    int  __ms_stat(char *pn, struct ms_stat *st);
    int  __ms_FILE_fstat(FILE* f, ms_stat *st);
    int  __ms_free(void*);

}
#endif

int _readdir(std::vector<std::string> & result, const std::string  &dir);

class st_ProgressWindow;
std::string in_pathname(const std::string &msg);
int read_one_out_message_header(std::map<std::string, std::map<std::string, std::string> >& to, const std::string &message);

void rename_gz2FILE(const std::string &pn, const std::string &pnto);
void rename_file(const std::string & pn, const std::string & pnto);

std::string messages_dat();

bool is_file_exists(const std::string &pathname);
long get_file_length(const std::string &pathname);
long get_file_length_with_RF(const std::string &pathname);

#ifndef WIN32
#include <zlib.h>
#endif

int chk_dir_or_create_it(const char *dir);
void check_path_wfn(const std::string & pathname);
std::string GetHostIP(const std::string &host);

#endif
