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

#include "myQ.h"

Define_Module(MyQ);

void MyQ::initialize()
{
    queue.setName("queue");
}

void MyQ::handleMessage(cMessage *msg)
{
    //int i;
    int ql;
    ql = queue.getLength();
    cModule *network = getParentModule();
    if (msg->arrivedOn("rxPackets")){
        EV<<"msg->arrivedOn(rxPackets)"<<endl;
        if (msg->arrivedOn("rxPackets", 0))
             {
                double time = msg->getCreationTime().dbl();
                EV<<"msg->getCreationTime().dbl()"<<time<<endl; //deci aici trb sa iei timpul la care o sa se trimit
                network->par("totalDelayHP").setDoubleValue(time);
             }
        queue.insert(msg);
    } else if (msg->arrivedOn("rxScheduling")){
        //read parameters from msg
        delete msg;
        //empty the queue !
        if(!queue.isEmpty()){ //sa trimita un singur pachet
            EV<<"golim coada"<<endl;
          msg = (cMessage *)queue.pop();
          send(msg, "txPackets");
        }
    }
}
