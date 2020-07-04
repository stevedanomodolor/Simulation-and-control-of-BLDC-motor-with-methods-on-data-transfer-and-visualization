/*
 * @file petrinet.c
 * @brief Implementation of coloured petrinet
 *
 * This file contains the implementation of general purpose coloured petrinets with completely customiseable barrier conditions,
 * consumption behaviours and production behaviours. It is based on cgraph which is graph library to construct and modify graphs
 * at run-time.
 *
 * (c) Filip Reniers (KU Leuven) 28.01.20
 *
 */

#include "petrinet.h"

void agclose_(Agraph_t * g) // TODO reconsider
{
    Agnode_t *n, *next_n;

    for (n = agfstnode(g); n; n = next_n) {
        next_n = agnxtnode(g, n);
        agdelnode(g, n);
    }
}

petrinet_t *init_petrinet(char *petrinet_name) {
    petrinet_t *g = agopen(petrinet_name, Agdirected, NULL);
    agattr(g, AGNODE, "type", "");
    agattr(g, AGNODE, "number_of_token_types", "");
    return g;
}

void destroy_petrinet(petrinet_t *p) { // TODO DO THIS IN CLEANUP
    for (Agnode_t *t = agfstnode(p); t; t = agnxtnode(p, t)) {
        if (!strcmp(agget(t, "type"), "transition")) {
            delete_behaviour(t);
        } else if (!strcmp(agget(t, "type"), "place")) {
            Agrec_t *i = t->base.data->next;
            char *number_of_token_types_str = agget(t, "number_of_token_types");
            int number_of_token_types = (number_of_token_types_str ? atoi(number_of_token_types_str) : 0);
            for (int count = 0; count < number_of_token_types; count++) {
                agdelrec(t, ((count_of_token_t *) i)->token_type);
                i = i->next;
            }
        }
    }
//    agclose_(p); // todo questionable segmentation fault
}

place_t *create_place(petrinet_t *p, char *name) {
    place_t *p1 = agnode(p, name, TRUE);
    agset(p1, "type", "place");
    agset(p1, "number_of_token_types", "0");
    return p1;
}

transition_t *create_transition(petrinet_t *p, char *name) {
    transition_t *t1 = agnode(p, name, TRUE);
    agset(t1, "type", "transition");
    return t1;
}

void add_behaviour(transition_t *t1, transition_behaviour_t *b1) {
    transition_attribute_t *rec = NULL;
    if (rec = (transition_attribute_t *) aggetrec(t1, "behaviour",
                                                  0)) {// there are already behaviours in a transition attribute
        rec->number_of_behaviours++;
        transition_behaviour_t *b_new = (transition_behaviour_t *) malloc(
                rec->number_of_behaviours * sizeof(transition_behaviour_t));
        memcpy(b_new, rec->behaviours, (rec->number_of_behaviours - 1) * sizeof(transition_behaviour_t));
        free(rec->behaviours);
        b_new[rec->number_of_behaviours - 1] = *b1;
        rec->behaviours = b_new;

    } else {
        transition_attribute_t *rec = (transition_attribute_t *) agbindrec(t1, "behaviour",
                                                                           sizeof(transition_attribute_t), 0);
        if (rec) {
            rec->number_of_behaviours = 1;
            transition_behaviour_t *b_new = (transition_behaviour_t *) malloc(sizeof(transition_behaviour_t));
            *b_new = *b1;
            rec->behaviours = b_new;
        }
    }
}

void delete_behaviour(transition_t *t1) {

    transition_attribute_t *rec = NULL;
    if (rec = (transition_attribute_t *) aggetrec(t1, "behaviour", 0)) {
        if (rec->number_of_behaviours > 0) {
            free(rec->behaviours);
            rec->number_of_behaviours = 0;
        }
        agdelrec(t1, "behaviour");
    }

}

int create_token_type(place_t *p, char *typename, int n) {
    count_of_token_t *rec = (count_of_token_t *) agbindrec(p, typename, sizeof(count_of_token_t), 0);
    if (rec) {
        rec->count = n;
        snprintf(rec->token_type, MAX_LENGTH_TOKEN_TYPE_NAME, "%s", typename);

        char *number_of_token_types = agget(p, "number_of_token_types");
        char number[4];
        snprintf(number, 4, "%d", (number_of_token_types ? atoi(number_of_token_types) : 0) + 1);
        agset(p, "number_of_token_types", number);
        return 0;
    } else {
        return 1;
    }
}

void delete_token_type(place_t *p, char *typename) {
    count_of_token_t *rec = (count_of_token_t *) aggetrec(p, typename, 0);
    if (rec) {
        agdelrec(p, typename);
        char *number_of_token_types = agget(p, "number_of_token_types");
        char number[4];
        snprintf(number, 4, "%d", (number_of_token_types ? atoi(number_of_token_types) : 0) - 1);
        agset(p, "number_of_token_types", number);
    }
    assert ((count_of_token_t *) aggetrec(p, typename, 0) == NULL);
}

void add_tokens(place_t *p, char *typename, int n) {
    count_of_token_t *rec = NULL;
    if (rec = (count_of_token_t *) aggetrec(p, typename, 0)) {
        rec->count += n;
    } else {
        create_token_type(p, typename, n);
    }
}

int delete_tokens(place_t *p, char *typename, int n) {
    count_of_token_t *rec = (count_of_token_t *) aggetrec(p, typename, 0);
    if (rec) { // the token type exists
        if (n < rec->count) {
            rec->count -= n;
        } else {
            delete_token_type(p, typename);
            if (n > rec->count) // remove more tokens than there are available
                return 1;
        }
        return 0;
    } else {
        return 1;
    }
}

void trigger_petrinet(petrinet_t *p) {
    bool still_transitions_to_trigger = true;
    while (still_transitions_to_trigger) {
        still_transitions_to_trigger = false;
        for (Agnode_t *t = agfstnode(p); t; t = agnxtnode(p, t)) {
            if (!strcmp(agget(t, "type"), "transition")) {
                transition_attribute_t *rec = NULL;
                rec = (transition_attribute_t *) aggetrec(t, "behaviour", 0); // todo handle null
                for (int i = 0; i < rec->number_of_behaviours; i++) {
                    if (rec->behaviours[i].condition(t)) {
                        still_transitions_to_trigger = true;
                        rec->behaviours[i].consumption_behaviour(t);
                        rec->behaviours[i].production_behaviour(t);
                        break;
                    }
                }
            }
        }
    }

}

void fprint_marking(petrinet_t *p, FILE *pipe) {
    fprintf(pipe, "\n");
    for (Agnode_t *n = agfstnode(p); n; n = agnxtnode(p, n)) {
        if (!strcmp(agget(n, "type"), "place")) {
            fprintf(pipe, "place %s has:\n", agnameof(n));
            Agrec_t *i = n->base.data->next;
            char *number_of_token_types_str = agget(n, "number_of_token_types");
            int number_of_token_types = (number_of_token_types_str ? atoi(number_of_token_types_str) : 0);
            for (int count = 0; count < number_of_token_types; count++) {
                fprintf(pipe, "%d tokens of type %s\n", ((count_of_token_t *) i)->count,
                        ((count_of_token_t *) i)->token_type);
                i = i->next;
            }
        }

    }
    fprintf(pipe, "\n");
}

void print_marking(petrinet_t *p) {
    fprint_marking(p, stdout);
}

/* replace all spaces by an underscore */
char *encode_string(char *st) {
    char *dup = strdup(st);
    char *str = dup;
    while (*str != '\0') {
        if (*str == ' ') {
            *str = '_';
        }
        str++;
    }
    return dup;
}

void fprintf_petrinet(petrinet_t *p, FILE *f) {
    fprintf(f, "digraph \"%s\"\n{\n", agnameof(p));
    for (Agnode_t *n = agfstnode(p); n; n = agnxtnode(p, n)) {
        if (!strcmp(agget(n, "type"), "transition")) {
            // t4 [type=transition];
            Agrec_t *i = n->base.data->next;
            fprintf(f, "%s [type=transition,shape=box];\n", agnameof(n));
        }
    }
    for (Agnode_t *n = agfstnode(p); n; n = agnxtnode(p, n)) {
        if (!strcmp(agget(n, "type"), "place")) {
            // thread_configures   [number_of_token_types=0,type=place,label=<eventloop configures<br/><br/>1 of blacktoken>];
            Agrec_t *i = n->base.data->next;
            char *number_of_token_types_str = agget(n, "number_of_token_types");
            int number_of_token_types = (number_of_token_types_str ? atoi(number_of_token_types_str) : 0);
            fprintf(f, "%s [number_of_token_types=%d,type=place,label=<%s", agnameof(n),
                    number_of_token_types, agnameof(n));
            for (int count = 0; count < number_of_token_types; count++) {
                //todo different token types, different colors, ...
                //char *blackTokenBullet = "&#8226;";
                count_of_token_t *tokens = (count_of_token_t *) i;
                fprintf(f, "<br/>%d of %s", tokens->count, tokens->token_type);
                i = i->next;
            }
            fprintf(f, ">];\n");
        }
    }
    for (Agnode_t *n = agfstnode(p); n; n = agnxtnode(p, n)) {
        if (!strcmp(agget(n, "type"), "transition")) {
            int index;
            // in
            index = 0;
            for (Agedge_t *e = agfstin(p, n); e; e = agnxtin(p, e)) {
                Agnode_t *in = agtail(e);
                // t1 -> thread_configures [key=1];
                fprintf(f, "%s -> %s [key=%d];\n", agnameof(in), agnameof(n), ++index);
            }
            // out
            index = 0;
            for (Agedge_t *e = agfstout(p, n); e; e = agnxtout(p, e)) {
                Agnode_t *out = aghead(e);
                // t1 -> thread_configures [key=1];
                fprintf(f, "%s -> %s [key=%d];\n", agnameof(n), agnameof(out), ++index);
            }
        }
    }
    fprintf(f, "}");
}

int write_petrinet(petrinet_t *p) {
    FILE *f;
    int err = 0;
    char command[500];
    if (!p)
        return false;

    char *filename = encode_string(agnameof(p));
    sprintf(command, "dot -Tpdf -Gorientation=portrait -o %s.pdf ", filename);
    free(filename);
    f = popen(command, "w");
    if (!f)
        return false;
    //    err = agwrite(p, f);
    fprintf_petrinet(p, f);
    pclose(f);
    return (!err);
}

Agnode_t *get_node(Agraph_t *p, char *id) {
    for (Agnode_t *n = agfstnode(p); n; n = agnxtnode(p, n)) {
        if (!strcmp(agnameof(n), id))
            return n;
    }
    return NULL;
}

place_t *get_place(petrinet_t *p, char *id) {
    //return agnode(p, id, FALSE);
    return get_node(p, id);
}

transition_t *get_transition(petrinet_t *p, char *id) {
    //return agnode(p, id, FALSE);
    return get_node(p, id);
}

int get_number_of_tokens(place_t *p, char *typename) {
    count_of_token_t *rec = (count_of_token_t *) aggetrec(p, typename, 0);
    if (rec) {
        return rec->count;
    } else {
        return 0;
    }
}

bool check_number_of_tokens(place_t *p, char *typename, int n) {

    int number_of_tokens = get_number_of_tokens(p, typename);
    if (number_of_tokens >= n)
        return true;
    else
        return false;
}

bool has_any_tokens(place_t *p) {
    return (atoi(agget(p, "number_of_token_types")) ? true : false);
}

void consume_nothing(transition_t *t) {
    return;
}

void produce_nothing(transition_t *t) {
    return;
}

bool cond_token_type_in_all_incoming_places(transition_t *t, char *token_type) {
    // check whether there is  a token_type token in all incoming places
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        if (!check_number_of_tokens(n, token_type, 1)) {
            return false;
        }
    }
    return true;
}

void consume_token_in_all_incoming_places(transition_t *t, char *token_name) {
    // consume a token_type token from all incoming places
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        delete_tokens(n, token_name, 1);
    }
}

void produce_token_in_all_outgoing_places(transition_t *t, char *token_name) {
    // produce a token_type token in all incoming places
    for (Agedge_t *e = agfstout(agraphof(t), t); e; e = agnxtout(agraphof(t), e)) {
        Agnode_t *n = aghead(e);
        add_tokens(n, token_name, 1);
    }
}

bool cond_token_in_all_but_one_place(transition_t *t, char *place_name, char *token_type) {
    // check whether there is a token_type token in all but 1 of the incoming places
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        if (!strcmp(agnameof(n), place_name)) {
            if (check_number_of_tokens(n, token_type, 1))
                return false;
        } else {
            if (!check_number_of_tokens(n, token_type, 1)) {
                return false;
            }
        }
    }
    return true;
}

void consume_token_from_all_but_1_place(transition_t *t, char *place_name, char *token_name) {
    // consume a token_type token from all but 1 of the incoming places
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        if (!strcmp(agnameof(n), place_name))
            continue;
        delete_tokens(n, token_name, 1);
    }
}

void produce_token_in_all_but_1_place(transition_t *t, char *place_name, char *token_name) {
    // produce a token_type token in all but 1 of the incoming places
    for (Agedge_t *e = agfstout(agraphof(t), t); e; e = agnxtout(agraphof(t), e)) {
        Agnode_t *n = aghead(e);
        if (!strcmp(agnameof(n), place_name))
            continue;
        add_tokens(n, token_name, 1);
    }
}

bool cond_token_in_one_place(transition_t *t, char *place_name, char *token_name) {
    // Check whether there is a token_type token in one place
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        if (!strcmp(agnameof(n), place_name)) {
            if (!check_number_of_tokens(n, token_name, 1)) {
                return false;
            }
        }
    }
    return true;
}

void consume_token_from_one_place(transition_t *t, char *place_name, char *token_name) {
    // consume a token_type token from one place
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        if (!strcmp(agnameof(n), place_name))
            delete_tokens(n, token_name, 1);
    }
}

void produce_token_in_one_place(transition_t *t, char *place_name, char *token_name) {
    // produce a token_type token in one place
    for (Agedge_t *e = agfstout(agraphof(t), t); e; e = agnxtout(agraphof(t), e)) {
        Agnode_t *n = aghead(e);
        if (!strcmp(agnameof(n), place_name))
            add_tokens(n, token_name, 1);
    }
}

bool cond_token_in_places(transition_t *t, char *place_name_list[], int number_of_places, char *token_name) {
    // Check whether there is a token_type token in every place of place_name_list
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        for (int i = 0; i < number_of_places; i++) {
            if (!strcmp(agnameof(n), place_name_list[i]))
                if (!check_number_of_tokens(n, token_name, 1))
                    return false;
        }
    }
    return true;
}

void consume_token_from_places(transition_t *t, char **place_name_list, int number_of_places, char *token_name) {
    // consume a token_type token from every place of place_name_list
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        for (int i = 0; i < number_of_places; i++) {
            if (!strcmp(agnameof(n), place_name_list[i]))
                delete_tokens(n, token_name, 1);
        }
    }
}

void produce_token_in_places(transition_t *t, char **place_name_list, int number_of_places, char *token_name) {
    // produce a token_type token in every place of place_name_list
    for (Agedge_t *e = agfstout(agraphof(t), t); e; e = agnxtout(agraphof(t), e)) {
        Agnode_t *n = aghead(e);
        for (int i = 0; i < number_of_places; i++) {
            if (!strcmp(agnameof(n), place_name_list[i]))
                add_tokens(n, token_name, 1);
        }
    }
}

bool cond_Black1(transition_t *t) {
    return cond_token_type_in_all_incoming_places(t, BLACK_TOKEN);
}

void consume_Black1(transition_t *t) {
    consume_token_in_all_incoming_places(t, BLACK_TOKEN);
}

void produce_Black1(transition_t *t) {
    produce_token_in_all_outgoing_places(t, BLACK_TOKEN);
}

bool cond_check_number_of_different_token_types(transition_t *t, char *black_token_place,
                                                char *place_collecting_different_types, int number_of_different_types) {
    int number_of_running_eventloops = 0;

    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);

        if (!strcmp(agnameof(n), black_token_place)) {
            if (!check_number_of_tokens(n, BLACK_TOKEN, 1))
                return false;
        } else if (!strcmp(agnameof(n), place_collecting_different_types)) {
            char *number_of_token_types_str = agget(n, "number_of_token_types");
            number_of_running_eventloops = (number_of_token_types_str ? atoi(number_of_token_types_str) : 0);
            if (number_of_running_eventloops != number_of_different_types)
                return false;
//            else
//                printf(" 4 TOKENS FOUND");
        }
    }
    return true;
}

void cvt_flag2token(petrinet_t *p, place_t *n, bool *flag, char *token_type) {
    if (*flag) {
        *flag = false;
        add_tokens(n, token_type, 1);
    }
}

void convert_flag_to_token(petrinet_t *p, char *place_name, bool *flag, char *token_type) {
    if (*flag) {
        place_t *n = get_place(p, place_name);
        if (n) {
            cvt_flag2token(p, n, flag, token_type);
        }
    }
}

void convert_flags_to_tokens(petrinet_t *p, char *place_name, bool *flag[], char *token_type[], int number_of_flags) {
    place_t *n = NULL;
    for (int i = 0; i < number_of_flags; i++) {
        if (*flag[i]) {
            if (n == NULL)
                n = get_place(p, place_name);
            if (n)
                cvt_flag2token(p, n, flag[i], token_type[i]);
        }
    }
}

void cvt_token2flag(petrinet_t *p, place_t *n, char *token_type, bool *flag) {
    if (check_number_of_tokens(n, token_type, 1)) {
        *flag = true;
        delete_tokens(n, token_type, 1);
    }
}

void convert_token_to_flag(petrinet_t *p, char *place_name, char *token_type, bool *flag) {
    place_t *n = get_place(p, place_name);
    if (n)
        cvt_token2flag(p, n, token_type, flag);
}

void convert_tokens_to_flags(petrinet_t *p, char *place_name, char *token_type[], bool *flag[], int number_of_tokens) {
    place_t *n = get_place(p, place_name);
    if (n)
        for (int i = 0; i < number_of_tokens; i++)
            cvt_token2flag(p, n, token_type[i], flag[i]);
}

void flag_tracks_token(petrinet_t *p, char *place_name, char *token_type, bool *flag) {
    place_t *n = get_place(p, place_name);
    if (n)
        *flag = check_number_of_tokens(n, token_type, 1);
}

void flags_track_tokens(petrinet_t *p, char *place_name, char *token_type[], bool *flag[], int number_of_tokens) {
    place_t *n = get_place(p, place_name);
    if (n) {
        for (int i = 0; i < number_of_tokens; i++) {
            *flag[i] = check_number_of_tokens(n, token_type[i], 1);
        }
    }
}

void token_tracks_flag(petrinet_t *p, char *place_name, bool *flag, char *token_type) {
    place_t *n = get_place(p, place_name);
    if (n) {
        bool token_in_place = check_number_of_tokens(n, token_type, 1);
        if (token_in_place && !(*flag))
            delete_tokens(n, token_type, 1);
        else if (!token_in_place && *flag)
            add_tokens(n, token_type, 1);
    }
}

void tokens_track_flags(petrinet_t *p, char *place_name, bool *flag[], char *token_type[], int number_of_flags) {
    place_t *n = get_place(p, place_name);
    if (n) {
        for (int i = 0; i < number_of_flags; i++) {
            bool token_in_place = check_number_of_tokens(n, token_type[i], 1);
            if (token_in_place && !(*flag[i]))
                delete_tokens(n, token_type[i], 1);
            else if (!token_in_place && *flag[i])
                add_tokens(n, token_type[i], 1);
        }
    }
}


































