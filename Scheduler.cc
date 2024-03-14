//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Scheduler.h"
#include "myQ.h"
Define_Module(Scheduler);

Scheduler::Scheduler()
{
    selfMsg = nullptr;
    hp_packs = 0;
    totalDelayHP = 0;
}

Scheduler::~Scheduler()
{
    cancelAndDelete(selfMsg);
}


void Scheduler::initialize()
{
    NrUsers = par("gateSize").intValue();
    cModule *parent = getParentModule();
    selfMsg = new cMessage("selfMsg");
       scheduleAt(simTime(), selfMsg);
}

void Scheduler::handleMessage(cMessage *msg)
{
  //  int userWeights[NrUsers];
   // if (msg == selfMsg){
        //for(int i =0;i<NrUsers;i++){
          //  cMessage *cmd = new cMessage("cmd");
            //send(cmd,"txScheduling",i);
                cMessage *cmd = new cMessage("mesaj de la schedluer");
                cModule *network = getParentModule();
                EV<<network<<endl;

                MyQ *hpq = check_and_cast<MyQ*>(network->getSubmodule("hpq"));
                MyQ *mpq = check_and_cast<MyQ*>(network->getSubmodule("mpq"));
                MyQ *lpq = check_and_cast<MyQ*>(network->getSubmodule("lpq"));

                int hp_length = hpq->queue.getLength();
                int mp_length = mpq->queue.getLength();
                int lp_length = lpq->queue.getLength();

                int hp_weigth = network->par("w_HP");
                int mp_weigth = network->par("w_MP");
                int lp_weigth = network->par("w_LP");

                double totalDelayHP = network->par("totalDelayHP");
                double averageDelayHP = network->par("averageDelayHP");

                EV<<"HP are lungimea "<<hp_length<<" si weigth-ul "<<hp_weigth<<endl;
                EV<<"MP are lungimea "<<mp_length<<" si weigth-ul "<<mp_weigth<<endl;
                EV<<"LP are lungimea "<<lp_length<<" si weigth-ul "<<lp_weigth<<endl;

                int hp_cont = hp_length * hp_weigth;
                int mp_cont = mp_length * mp_weigth;
                int lp_cont = lp_length * lp_weigth;

                int nrHP = network->par("nrHP");

                EV<<"pachete high prio: "<<hp_packs<<endl;

                int win = std::max(hp_cont, std::max(mp_cont, lp_cont));

                //WRR
                if((!hpq->queue.isEmpty()) && (win == hp_cont) )
                {

                    send(cmd, "txScheduling", 0);
                    hp_packs++;
                    network->par("nrHP").setIntValue(hp_packs);

                   // nrHP.setLongValue(hp_packs);

                }else if((!mpq->queue.isEmpty()) && (win == mp_cont))
                {
                    send(cmd, "txScheduling", 1);
                    mp_packs++;


                }else if((!lpq->queue.isEmpty()) && (win == lp_cont)){

                    send(cmd, "txScheduling", 2);
                    lp_packs++;

                }

                // PRIO QUEUE
                  /*if (!hpq->queue.isEmpty()){
                        send(cmd, "txScheduling", 0);

                }else if (!mpq->queue.isEmpty()){
                        send(cmd, "txScheduling", 1);

                }else if (!lpq->queue.isEmpty()){
                        send(cmd, "txScheduling", 2);

                }*/



            //set parameter value, e.g., nr of blocks to be sent from the queue by user i
           // send(cmd,"txScheduling",i);
       // }


   // }


//}
        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);
   // }
}

void Scheduler::updateHighPriorityDelay(simtime_t delay) {
    totalDelayHP += delay;
}

void Scheduler::finish() {

    EV << "Average delay for high priority packets: " << (totalDelayHP / hp_packs) << endl;
}
