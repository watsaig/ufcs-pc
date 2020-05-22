#include "testroutines.h"
#include "testcommunicator.h"

int main(int argc, char** argv)
{
   int status = 0;
   {
      TestCommunicator tc;
      status |= QTest::qExec(&tc, argc, argv);
   }

   {
      TestRoutines tc;
      status |= QTest::qExec(&tc, argc, argv);
   }

   return status;
}
