
#include <stdio.h>
#include <stdlib.h>
#include "board_elements/loader.h"
#include "pointer.h"
#include "code_core.h"
#include "code_elements/cell_behaviours.h"
#include "code_elements/opaques.h"
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define FRAME_RATE 60.0f

struct TidListCellStruct {
    pthread_t i;
    struct TidListCellStruct *next;
};
typedef struct TidListCellStruct *TidListCell;

static pthread_mutex_t output_lock;
void default_output(char* str) {
    pthread_mutex_lock(&output_lock);
    printf("%s", str);
    pthread_mutex_unlock(&output_lock);
}

static pthread_mutex_t input_lock;
StackElement default_input(void) {
    pthread_mutex_lock(&input_lock);
    StackElement ret = 0;
    while(!ret) {
        char str[100];
        scanf("%s", str);
        char *temp;
        temp = strchr("0123456789", str[0]);
        if (temp) {
            int res = 0, i;
            bool valid = true;
            for (i = 0; i < 100 && str[i] != '\0' && valid; i++) {
                //printf("Input: %d\n", i);
                switch(str[i]) {
                    case '0': res *= 10; break;
                    case '1': res = res * 10 + 1; break;
                    case '2': res = res * 10 + 2; break;
                    case '3': res = res * 10 + 3; break;
                    case '4': res = res * 10 + 4; break;
                    case '5': res = res * 10 + 5; break;
                    case '6': res = res * 10 + 6; break;
                    case '7': res = res * 10 + 7; break;
                    case '8': res = res * 10 + 8; break;
                    case '9': res = res * 10 + 9; break;
                    default:
                        valid = false;
                        break;
                }
                if (valid) {
                    ret = e_from_int(res);
                }
            }
        } else {
            temp = strchr("abcdefghijklmnopqrstuvwxyz", str[0]);
            int res = 0, i;
            bool valid = true;
            for (i = 0; i < 100 && str[i] != '\0' && valid; i++) {
                temp = strchr("abcdefghijklmnopqrstuvwxyz", str[0]);
                if (!temp) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                StackString stk_str = str_from_str(str);
                ret = e_from_str(stk_str);
            }
        }
    }
    pthread_mutex_unlock(&input_lock);
    return ret;
}

typedef struct {
    Pointer p; Board board; LanguageBinding binding;
} ThreadInput;

void *pointer_run_loop(void* thread_input) {
    ThreadInput t = *((ThreadInput*)thread_input);
    while(t.p->running) {
        ray_trace(t.p, t.board, t.binding);
    }
    p_free(t.p);
    free(thread_input);
    return NULL;
}

int sleepf(float sec) {
    struct timespec ts;
    int res;

    if (sec < 0) {
        return -1;
    }

    ts.tv_sec = (long)sec;
    ts.tv_nsec = (long)((fmodf(sec, 1.0f)) * 1000000000);

    res = nanosleep(&ts, &ts);
    return res;
}

int main(int argsc, char** argsv) {
    int ro = pthread_mutex_init(&output_lock, NULL);
    int ri = pthread_mutex_init(&input_lock, NULL);
    if (ro != 0 || ri != 0) {
        printf("Error %d or %d when initialising mutexes.\n", ro, ri);
        return 0;
    }

    bool vsync = false;
    if (argsc != 2 && argsc != 3) {
        return 0;
    }
    int file_index = 1;
    if (argsc == 3) {
        if (strcmp(argsv[1], "-v ")) {
            vsync = true;
            file_index = 2;
            printf("Vsee enabled\n");
        } else {
            return 0;
        }
    }
    FILE *fp;
    if (!(fp = fopen(argsv[file_index], "r"))) {
        printf("failed to find file %s\n", argsv[file_index]);
        return 0;
    }
    //printf("Loading file\n");
    PointerList pl = p_list_new();
    Board board = load_file(fp, pl, default_output, default_input);
    fclose(fp);

    //printf("Creating language bindings\n");
    LanguagePair default_language[] = {
        {'a', a_func}, {'b', b_func}, {'c', c_func}, {'d', d_func}, {'e', e_func},
        {'f', f_func}, {'g', g_func}, {'h', h_func}, {'i', i_func}, {'j', j_func},
        {'k', k_func}, {'l', l_func}, {'m', m_func}, {'n', n_func}, {'o', o_func},
        {'p', p_func}, {'q', q_func}, {'r', r_func}, {'s', s_func}, {'t', t_func},
        {'u', u_func}, {'v', v_func}, {'w', w_func}, {'x', x_func}, {'y', y_func},
        {'z', z_func},
        {'A', cap_a_func},
        {'D', cap_d_func},
        {'M', cap_m_func},
        {'O', omirror_func},
        {'P', cap_p_func},
        {'S', cap_s_func},
        {'Y', cap_y_func},
        {'%', percent_func},
        {'@', at_func},
        {'X', xmirror_func},
        {'+', plusmirror_func},
        {'?', condmirror_func},
        {'0', _0_func},
        {'1', _1_func},
        {'2', _2_func},
        {'3', _3_func},
        {'4', _4_func},
        {'5', _5_func},
        {'6', _6_func},
        {'7', _7_func},
        {'8', _8_func},
        {'9', _9_func},
        {'-', hmirror_func},
        {'\\', bmirror_func},
        {'|', vmirror_func},
        {'/', fmirror_func},
        {'$', dollar_func},
        {'\n', default_end},
        {'!', halt_end},
        {'#', output_end},
        {'I', input_end},
        {'<', left_end},
        {'>', right_end},
        {'^', up_end},
        {':', jump_end},
        {'V', down_end},
        {'~', bind_end},
        {'W', write_end},
        {'\0', 0},
    };
    LanguageBinding binding = create_binding(default_language);

    if (vsync) {
        PointerListCell c;
        while(1) {
            clock_t start = clock();
            bool halted = true;
            c = pl->l;
            while(c) {
                ray_trace(c->p, board, binding);
                if (c->p->running) {
                    halted = false;
                }
                c = c->next;
            }
            if (halted) break;
            float time = 1.0 * (clock() - start) / (float)CLOCKS_PER_SEC;
            sleepf((1 / FRAME_RATE) - time);
        }
        c = pl->l;
        while(c) {
            p_free(c->p);
            c = c->next;
        }
        p_list_free(pl);

    } else {
        PointerListCell c = pl->l;
        TidListCell tid_list = 0, next_tid;
        while(c) {
            TidListCell tidcell = malloc(sizeof(struct TidListCellStruct));
            *tidcell = (struct TidListCellStruct){0, tid_list};
            tid_list = tidcell;
            ThreadInput *thread_input = malloc(sizeof(ThreadInput));
            *thread_input = (ThreadInput){c->p, board, binding};
            pthread_create(&tidcell->i, NULL, pointer_run_loop, thread_input);
            c = c->next;
        }
        p_list_free(pl);
        while(tid_list) {
            pthread_join(tid_list->i, NULL);
            next_tid = tid_list->next;
            free(tid_list);
            tid_list = next_tid;
        }
    }
}