#include "Arduino.h"
#include "SerialNode.h"


//contructor
SerialNode::SerialNode()
{
}

void SerialNode::init()
{
  
  //Node::init();
}

boolean SerialNode::isActive(){
  // Detect if a node is attached
  return false;
}

short SerialNode::processRequest(byte* request, byte* answer)
{
  byte nodeID = request[0];
  request[0] = 0x01;  // override nodeID
  // Send Request to Serial



  answer[0] = nodeID | 0x80;
}
