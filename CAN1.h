#ifndef CAN1_H
#define CAN1_H

/*
 * CAN_FRAME_STREAM
 * Class to store can messages upon arrival in MCP2521 buffer.
 * Called upon by Interrupt handler.
 */
class CanFrameStream {

    static const int buffsize = 3;
    can_frame cfBuffer[ buffsize ]; // struct from <mcp2515.h>
    int readIndex; //where to read next message
    int writeIndex; //where to write next message
    bool writeLock; //buffer full
  public:
    CanFrameStream()
      : readIndex( 0 ) , writeIndex( 0 ), writeLock( false )
    {};

    int put( can_frame & ); 
    int get( can_frame & );
};

inline int CanFrameStream::put( can_frame &frame ) {
  if ( writeLock )
    return 0; //buffer full
  cfBuffer[ writeIndex ] = frame;
  writeIndex = ( ++writeIndex ) % buffsize;
  if ( writeIndex == readIndex)
    writeLock = true; //cannot write more
  return 1;
}

inline int CanFrameStream::get( can_frame &frame ) {
  if ( !writeLock && ( readIndex == writeIndex ) )
    return 0; //empty buffer
  if ( writeLock && ( readIndex == writeIndex ) )
    writeLock = false; //release lock
  frame = cfBuffer[ readIndex ];
  readIndex = ( ++readIndex ) % buffsize;
  return 1;
}

/*
 * CAN_FSM_Buffer
 * Container class for can message distribution among System FSMs
 * Called upon during runtime
 */
struct instruction{
  unsigned char data[3]; //only 3 bytes need saving since recipientId can be discarded
};

struct instructionFloat{
  unsigned char data[6]; // since recipientId is in the middle(byte 3), no extraction is done and recipient Id is also stored. Hence 6 bytes
};

class CanBuffer {

    static const int bufferSize = 2;
    instruction fsmBuffer[bufferSize];

    int readIndex = 0;
    int writeIndex = 0;
    bool flagFull = false; // buffer full
    bool flagEmpty = true;
    

  public:
  /*
    bool getFlagEmpty() {
      return flagEmpty;
    }
    bool getFlagFull() {
      return flagFull;
    }
  */
    bool write(instruction &);
    bool read(instruction &);

};

inline bool CanBuffer::write(instruction &instruct){
  if(flagFull)
    return false;
  if(flagEmpty)
    flagEmpty = false;
  fsmBuffer[writeIndex] = instruct;
  writeIndex = ( ++writeIndex ) % bufferSize;
  if (writeIndex== readIndex)
    flagFull = true;
  return true;
}

inline bool CanBuffer::read(instruction &instruct){
  if(flagEmpty)
    return false;
  if(flagFull)
    flagFull = false;
  instruct = fsmBuffer[readIndex];
  readIndex = (++readIndex) % bufferSize;
  if(readIndex == writeIndex)
    flagEmpty = true;
  return true;
}

class CanFloatBuffer {

    static const int bufferSize = 2;
    instructionFloat fsmBuffer[bufferSize];

    int readIndex = 0;
    int writeIndex = 0;
    bool flagFull = false; // buffer full
    bool flagEmpty = true;
    

  public:
 
    bool write(instructionFloat &);
    bool read(instructionFloat &);

};

inline bool CanFloatBuffer::write(instructionFloat &instruct){
  if(flagFull)
    return false;
  if(flagEmpty)
    flagEmpty = false;
  fsmBuffer[writeIndex] = instruct;
  writeIndex = ( ++writeIndex ) % bufferSize;
  if (writeIndex== readIndex)
    flagFull = true;
  return true;
}

inline bool CanFloatBuffer::read(instructionFloat &instruct){
  if(flagEmpty)
    return false;
  if(flagFull)
    flagFull = false;
  instruct = fsmBuffer[readIndex];
  readIndex = (++readIndex) % bufferSize;
  if(readIndex == writeIndex)
    flagEmpty = true;
  return true;
}



#endif
