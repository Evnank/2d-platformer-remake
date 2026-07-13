#include "GLOBALS.h"

GLOBAL_CONSTANTS CONSTANTS_GLOBAL;
GLOBAL_VARIABLES VARIABLES_GLOBAL;
ASSETS GLOBAL_ASSETS;

std::string is_entity_on_string(){
    if (VARIABLES_GLOBAL.editor_is_entity){
        return "mode: ENTITY";
    } else {
        return "mode: BLOCK";
    }
}