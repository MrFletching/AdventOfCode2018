#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_GROUPS 10
#define MAX_ARMY_NAME 15
#define MAX_LINE_LENGTH 200

#define ATTACK_TYPE_COUNT 5

typedef enum {
    ATTACK_TYPE_INVALID     = 0,
    ATTACK_TYPE_SLASHING    = (1 << 0),
    ATTACK_TYPE_FIRE        = (1 << 1),
    ATTACK_TYPE_RADIATION   = (1 << 2),
    ATTACK_TYPE_COLD        = (1 << 3),
    ATTACK_TYPE_BLUDGEONING = (1 << 4)
} AttackType;

const char *ATTACK_TYPE_STRINGS[ATTACK_TYPE_COUNT] = {"slashing", "fire", "radiation", "cold", "bludgeoning"};

struct Army_s;

typedef struct Group_s {
    struct Army_s *army;
    int id;
    int unit_count;
    int hit_points;
    int damage;
    AttackType attack_type;
    int initiative;
    int weak_to;
    int immune_to;
    struct Group_s *chosen_target;
    bool has_been_targeted;
} Group;

typedef struct Army_s {
    char name[MAX_ARMY_NAME];
    Group groups[MAX_GROUPS];
    int group_count;
} Army;

AttackType str_to_attack_type(char *str) {
    for(int i = 0; i < ATTACK_TYPE_COUNT; i++) {
        if(strcmp(str, ATTACK_TYPE_STRINGS[i]) == 0) {
            return (1 << i);
        }
    }
    
    return ATTACK_TYPE_INVALID;
}

void group_init(Group *group, Army *army) {
    group->army = army;
    group->id = 0;
    group->unit_count = 0;
    group->hit_points = 0;
    group->damage = 0;
    group->attack_type = 0;
    group->initiative = 0;
    group->weak_to = 0;
    group->immune_to = 0;
    group->chosen_target = NULL;
    group->has_been_targeted = false;
}

int group_effective_power(Group *group) {
    return group->unit_count * group->damage;
}

void army_print(Army *army) {
    printf("%s:\n", army->name);
    
    int group_count = 0;
    
    for(int i = 0; i < army->group_count; i++) {
        if(army->groups[i].unit_count == 0) {
            continue;
        }
        
        printf("Group %d contains %d units\n", army->groups[i].id, army->groups[i].unit_count);
        group_count++;
        // printf("Group %d\n", i+1);
        // printf("- Units: %d\n", army->groups[i].unit_count);
        // printf("- Hit points: %d\n", army->groups[i].hit_points);
        // printf("- Damage: %d\n", army->groups[i].damage);
        // printf("- Attack Type: %d\n", army->groups[i].attack_type);
        // printf("- Initiative: %d\n", army->groups[i].initiative);
        // printf("- Weak To: %d\n", army->groups[i].weak_to);
        // printf("- Immune To: %d\n", army->groups[i].immune_to);
    }
    
    if(group_count == 0) {
        printf("No groups remain.\n");
    }
}

void group_read_in_brackets_part(Group *group, char *part) {
    //printf("Part: %s\n", part);
    
    char type[MAX_LINE_LENGTH];
    sscanf(part, "%s", type);
    
    int *attribute;
    
    if(strcmp(type, "immune") == 0) {
        part += strlen("immune to ");
        attribute = &(group->immune_to);
    } else if(strcmp(type, "weak") == 0) {
        part += strlen("weak to ");
        attribute = &(group->weak_to);
    } else {
        assert(false);
    }
    
    char *attack_token = strtok(part, ", ");
    
    do {
        AttackType type = str_to_attack_type(attack_token);
        *attribute |= type;
        
        attack_token = strtok(NULL, ", ");
    } while(attack_token != NULL);
}

void group_read_in_brackets(Group *group, char *in_brackets) {
    //printf("In brackets: %s\n", in_brackets);
    
    char *semicolon = strchr(in_brackets, ';');
    
    if(semicolon) {
        char first[MAX_LINE_LENGTH];
        strncpy(first, in_brackets, semicolon - in_brackets);
        first[semicolon - in_brackets] = '\0';
        
        group_read_in_brackets_part(group, first);
        group_read_in_brackets_part(group, semicolon + 2);
    } else {
        group_read_in_brackets_part(group, in_brackets);
    }
    
}

void group_read(Group *group, Army *army, char *line, int group_id) {
    group_init(group, army);
    group->id = group_id;
    
    int line_position;
    sscanf(line, "%d units each with %d hit points%n", &(group->unit_count), &(group->hit_points), &line_position);
    
    char *bracket_open;
    char *bracket_close;
    char in_brackets[MAX_LINE_LENGTH];
    
    bracket_open  = strchr(line, '(');
    bracket_close = strchr(line, ')');
    
    if(bracket_open && bracket_close) {
        int brackets_size = bracket_close - bracket_open - 1;
        
        strncpy(in_brackets, bracket_open + 1, brackets_size);
        in_brackets[brackets_size] = '\0';
        group_read_in_brackets(group, in_brackets);
        bracket_close++;
    } else {
        bracket_close = line + line_position;
    }
    
    char attack_type[MAX_LINE_LENGTH];
    
    sscanf(bracket_close+1, "with an attack that does %d %s damage at initiative %d", &(group->damage), attack_type, &(group->initiative));
    
    group->attack_type = str_to_attack_type(attack_type);
}

int group_calculate_damage(Group *group_attacking, Group *group_defending) {
    int damage = group_effective_power(group_attacking);
    
    AttackType attack_type = group_attacking->attack_type;
    
    if((group_defending->immune_to & attack_type) == attack_type) {
        return 0;
    }
    
    if((group_defending->weak_to & attack_type) == attack_type) {
        return damage *2;
    }
    
    return damage;
}

void army_read(Army *army, FILE *fp) {
    army->group_count = 0;
    char line[MAX_LINE_LENGTH] = "";
    
    fgets(line, MAX_LINE_LENGTH, fp);
    strtok(line, ":");
    strncpy(army->name, line, MAX_ARMY_NAME);
    
    while(fgets(line, MAX_LINE_LENGTH, fp)) {
        if(strcmp(line, "\n") == 0) {
            break;
        }
        
        Group *group = &(army->groups[army->group_count]);
        group_read(group, army, line, army->group_count+1);
        
        army->group_count++;
        
    }
}

void read_armies(Army *army_1, Army *army_2, char *filename) {
    FILE *fp = fopen(filename, "r");
    
    army_read(army_1, fp);
    army_read(army_2, fp);
    
    fclose(fp);
}

int group_cmp(const void *a, const void *b) {
    Group *group_a = (Group*) a;
    Group *group_b = (Group*) b;
    
    int group_a_effective_power = group_effective_power(group_a);
    int group_b_effective_power = group_effective_power(group_b);
    
    if(group_a_effective_power == group_b_effective_power) {
        return group_b->initiative - group_a->initiative;
    } else {
        return group_b_effective_power - group_a_effective_power;
    }
}

int group_cmp_initiative(const void *a, const void *b) {
    Group **group_a = (Group**) a;
    Group **group_b = (Group**) b;
    
    return (*group_b)->initiative - (*group_a)->initiative;
}

void army_sort_groups(Army *army) {
    qsort(army->groups, army->group_count, sizeof(Group), group_cmp);
}

void army_target_selection(Army *army_attacking, Army *army_defending) {

    for(int i = 0; i < army_attacking->group_count; i++) {
        if(army_attacking->groups[i].unit_count == 0) {
            continue;
        }
        
        Group *chosen_target = NULL;
        int max_damage = 0;
        int max_effective_power = 0;
        int max_initiative = 0;
        
        for(int j = 0; j < army_defending->group_count; j++) {
            if(army_defending->groups[j].has_been_targeted || army_defending->groups[j].unit_count == 0) {
                continue;
            }
            
            int damage = group_calculate_damage(&(army_attacking->groups[i]), &(army_defending->groups[j]));
            
            int effective_power = group_effective_power(&(army_attacking->groups[i]));
            int initiative = army_attacking->groups[i].initiative;
            
            printf("%s group %d would deal defending group %d %d damage\n", army_attacking->name, army_attacking->groups[i].id, army_defending->groups[j].id, damage);
            
            if( damage > 0 && (
                (damage > max_damage) ||
                ((damage == max_damage) && (effective_power > max_effective_power)) || 
                ((damage == max_damage) && (effective_power == max_effective_power) && (initiative > max_initiative))
            )) {
                chosen_target = &(army_defending->groups[j]);
                max_damage = damage;
                max_effective_power = effective_power;
                max_initiative = initiative;
            }
        }
        
        if(chosen_target != NULL) {
            army_attacking->groups[i].chosen_target = chosen_target;
            chosen_target->has_been_targeted = true;
            
            //printf("%s group %d will target defending group %d\n", army_attacking->name, army_attacking->groups[i].id, chosen_target->id);
        }
        
    }
}

int army_total_units(Army *army) {
    int total = 0;
    
    for(int i = 0; i < army->group_count; i++) {
        total += army->groups[i].unit_count;
    }
    
    return total;
}

void attack(Army *army_1, Army *army_2) {
    // Sort groups into initiative order
    Group *groups[MAX_GROUPS*2];
    int total_group_count = 0;
        
    for(int i = 0; i < army_1->group_count; i++) {
        if(army_1->groups[i].unit_count == 0 || army_1->groups[i].chosen_target == NULL) {
            continue;
        }
        
        groups[total_group_count++] = &(army_1->groups[i]);
    }
    
    for(int i = 0; i < army_2->group_count; i++) {
        if(army_2->groups[i].unit_count == 0 || army_2->groups[i].chosen_target == NULL) {
            continue;
        }
        
        groups[total_group_count++] = &(army_2->groups[i]);
    }
    
    qsort(groups, total_group_count, sizeof(Group*), group_cmp_initiative);
    
    // Attack!
    for(int i = 0; i < total_group_count; i++) {
        int damage = group_calculate_damage(groups[i], groups[i]->chosen_target);
        
        int units_killed = damage / groups[i]->chosen_target->hit_points;
        
        if(units_killed > groups[i]->chosen_target->unit_count) {
            units_killed = groups[i]->chosen_target->unit_count;
        }
        
        groups[i]->chosen_target->unit_count -= units_killed;
        
        printf("%s group %d attacks defending group %d, killing %d units\n", groups[i]->army->name, groups[i]->id, groups[i]->chosen_target->id, units_killed);
    }
}

void army_reset_target_selection(Army *army) {
    for(int i = 0; i < army->group_count; i++) {
        army->groups[i].has_been_targeted = false;
        army->groups[i].chosen_target = NULL;
    }
}

int main() {
    Army immune_system;
    Army infection;
    
    read_armies(&immune_system, &infection, "input.txt");
    
    int immune_system_units, infection_units;
    
    do {
        army_print(&immune_system);
        army_print(&infection);
        
        printf("\n");
        
        army_reset_target_selection(&infection);
        army_reset_target_selection(&immune_system);
        
        army_sort_groups(&infection);
        army_sort_groups(&immune_system);
        
        army_target_selection(&infection, &immune_system);
        army_target_selection(&immune_system, &infection);
        
        printf("\n");
        
        attack(&infection, &immune_system);
        
        printf("\n\n\n");
        
        immune_system_units = army_total_units(&immune_system);
        infection_units     = army_total_units(&infection);
        
    } while(immune_system_units != 0 && infection_units != 0);
    
    army_print(&immune_system);
    army_print(&infection);
    
    printf("\n");
    
    printf("Winning army units: %d\n", immune_system_units + infection_units);

    
    return 0;
}
