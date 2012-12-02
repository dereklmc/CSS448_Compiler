/* program sttest */

#define grades 5
#define size 4

typedef int int3D[];
//typedef cell *cellPtr;

typedef struct cell{
	int id;
	int3D info;
	cellPtr next;
    /* produce error: id exists */
};
typedef cell *cellPtr;
//typedef  DigitsetType code here!;

cellPtr list;
cellPtr newrec;
int3d mycount;
int classNum;

/* scopeEntry proc1 */
class proc1 {
    public:
        int mycount;
        int x1;
        int y;

        void call() { }
};
/* scopeExit proc1 */


class proc2 {
    public:
        /* scopeEntry proc2 */

        int3D mycount;
        int x2;
        int y;
        int z;
        //int y;/* produce error: y exists */

        class proc2a {
            public:
                /* scopeEntry proc2a */
                int3D mycount;
                int x2a;
                int y;
                int z;
                
                class proc2b {
                    public:
                        /* scopeEntry proc2b */
                        int x2b;
                        int y;

                        class proc2c {
                            public:
                                /* scopeEntry proc2c */

                                #define someConst 10
                                #define theConst 20

                                int x2c;
                                int y;
                                int x2c;/* produce error: x2c exists */

                                class func2d { // TODO
                                    public:    // TODO
                                        /* scopeEntry func2d */
                                        a;
                                        
                                        void call() {

                                        }
                                //}
                                /* scopeExit func2d */

                                void call() {

                                }

                        }
                        /* scopeExit proc2c */
                        
                        void call() {

                        }
                }
                /* scopeExit proc2b */
                    
                void call() {

                }
        }
        /* scopeExit proc2a */
                
        void call() {

        }
}
/* scopeExit proc2 */

void call() {

}


/* scopeExit example */

int main() {
    call();
    return 0;
}

