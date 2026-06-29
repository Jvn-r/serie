#include "ser_prfs.h"
#include "ser_proc.h"

int prfs_look(proc *p){
    if(prfs_stat(p) == -1)
        return -1;
    if(prfs_status(p) == -1)
        return -1;
    if(prfs_fd(p) == -1)
        return -1;
    return 0;
}

static inline void skip_spac(char **curs){
    while(**curs == ' ')
        (*curs)++;
}

int prfs_stat(proc *p){
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/stat", p->pid);

    FILE *f = fopen(path, "r");
    if(f == NULL)
        return -1;

    char buff[1024];
    if(fgets(buff, sizeof(buff), f) == NULL){
        fclose(f);
        return -1;
    }
    fclose(f);

    char *start_brac = strchr(buff, '(');
    char *end_brac = strrchr(buff, ')');
    
    if(!start_brac || !end_brac || end_brac <= start_brac)
        return -1; 

    size_t name_size = end_brac - (start_brac + 1);
    if(name_size > 15)                              //coz p->name is only char[16]
        name_size = 15;
    memcpy(p->name, start_brac + 1, name_size);
    p->name[name_size] = '\0';

    char *curs = end_brac + 1;
    skip_spac(&curs);

    p->state = *curs;
    curs++; 
    skip_spac(&curs);

    p->ppid = (pid_t)strtol(curs, &curs, 10);
    if(p->birth_ppid == -1)
        p->birth_ppid = p->ppid;
    skip_spac(&curs);

    p->alive = true;

    return 0;
}

int prfs_status(proc *p){
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", p->pid);

    FILE *f = fopen(path, "r");
    if(f == NULL)
        return -1;

    char line[256];

    p->tgid = -1;
    p->fd_coun = -1;

    while(fgets(line, sizeof(line), f) != NULL){
        if(strncmp(line, "Tgid:", 5) == 0){
            p->tgid = (pid_t)strtol(line + 5, NULL, 10);
        }else if(strncmp(line, "Uid:", 4) == 0){
            char *curs = line + 4;
            p->ruid = (uid_t)strtol(curs, &curs, 10);
            p->euid = (uid_t)strtol(curs, NULL, 10);
        }else if(strncmp(line, "Gid:", 4) == 0){
            char *curs = line + 4;
            p->rgid = (gid_t)strtol(curs, &curs, 10);
            p->egid = (gid_t)strtol(curs, NULL, 10);
        }else if(strncmp(line, "FDSize:", 7) == 0){
            p->fd_coun = (int)strtol(line + 7, NULL, 10);
            break;
        }
    }
    fclose(f);

    if(p->tgid == -1 || p->fd_coun == -1)
        return -1;
    return 0;
}

static DIR *prfs_open_dir(proc *p){
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/fd/", p->pid);
    DIR *dp = opendir(path);

    if(dp == NULL)
        return NULL;

    return dp;
}

void prfs_clean_fd(proc *p, int i){
    for(int j = 0; j < i; j++)
        free(p->fd_paths[j]);

    free(p->fd_paths);
    p->fd_paths = NULL;
}

int prfs_fd(proc *p){
    if(p->fd_coun == 0){
        p->fd_paths = NULL;
        return 0;
    }

    DIR *dp = prfs_open_dir(p);
    if(dp == NULL)
        return -1;

    struct dirent *dirs;
    int i = 0;

    if (p->fd_paths != NULL)
        prfs_clean_fd(p, p->fd_coun);

    p->fd_paths = malloc(p->fd_coun * sizeof(char *));
    if(p->fd_paths == NULL){
        closedir(dp);
        return -1; 
    }

    while((dirs = readdir(dp)) != NULL){
        if(i >= p->fd_coun)
            break;

        if(dirs->d_name[0] == '.')
            continue;

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "/proc/%d/fd/%s", p->pid, dirs->d_name);

        p->fd_paths[i] = malloc(PATH_MAX);
        if(p->fd_paths[i] == NULL){
            prfs_clean_fd(p, i);
            closedir(dp);
            return -1;
        }

        ssize_t bytes = readlink(path, p->fd_paths[i], PATH_MAX-1);

        if(bytes != -1){
            p->fd_paths[i][bytes] = '\0';
            i++;
        }else{
            free(p->fd_paths[i]);
            p->fd_paths[i] = NULL;
            continue;
        }
        if(bytes == PATH_MAX-1)
            printf("returned buffer may be truncated\n");
    }
    closedir(dp);
    p->fd_coun = i;

    return 0;
}

