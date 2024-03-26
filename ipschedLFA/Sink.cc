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

#include "Sink.h"
#include "Scheduler.h"

Define_Module(Sink);

void Sink::initialize()
{
    lifetimeSignal = registerSignal("lifetime");
    qtimeHP.setName("HP");
    qtimeMP.setName("MP");
    qtimeLP.setName("LP");

}

void Sink::handleMessage(cMessage *msg)
{
    simtime_t lifetime = simTime() - msg->getCreationTime();
    if (msg->arrivedOn("rxPackets",0)) {

        EV<<"SUNT IN SINK"<<endl;
        cModule *network = getParentModule();
           hp_packs++;
           EV<<"hp_packs: "<<hp_packs<<endl;
            double delayHP = lifetime.dbl();
            EV<<"delay for hp: "<< delayHP<<endl;
            total += delayHP;
            EV<<"total:"<<total<<endl;
            qtimeHP.record(delayHP);
            if (hp_packs == 10) {

                double averageDelayHP = total/hp_packs;
                network->par("averageDelayHP").setDoubleValue(averageDelayHP);

                EV << "Average delay for high priority packets: " << (total / hp_packs) << endl;

                cMessage *start_FLC = new cMessage("start_FLC");
                send(start_FLC, "start_FLC");

                total = 0;
                hp_packs = 0;
            }
        }
    if(msg->arrivedOn("rxPackets",1)){
        mp_packs++;
        double delayMP = lifetime.dbl();
        EV<<"delay for MP"<<delayMP<<endl;
        qtimeMP.record(delayMP);
    }
    if(msg->arrivedOn("rxPackets",2)){
            lp_packs++;
            double delayLP = lifetime.dbl();
            EV<<"delay for LP"<<delayLP<<endl;
            qtimeLP.record(delayLP);
        }
      EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
      emit(lifetimeSignal, lifetime);
      delete msg;
}
