#include"Procedure.h"

/*******************************************************************************
 *
 * Standard constructor inherited from Symbol. See Symbol.
 */
Procedure::Procedure(string name) :
        Symbol(name)
{
}

/*******************************************************************************
 *
 * Add a new parameter that must be given when calling the procedure. Also, 
 * checks if the parameter can be added. A parameter may not be able to be added
 * if a parameter with the same name already exits.
 *
 * @param param - the parameter to add.
 * @return true if the parameter can be added (does not already exist), 
 *          otherwise false.
 */
bool Procedure::addParameter(const Parameter &param)
{
    parameters.push_back(param);
}
