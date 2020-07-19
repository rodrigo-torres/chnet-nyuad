/*
 * daq_minimal_cpp.cpp
 *
 *  Created on: Oct 23, 2019
 *      Author: Rodrigo Torres
 */

#include "daq_director.h"

namespace maxrf::daq {
void DAQBridge::start_daq()
{
//  if (daq_mode == 2)
//  {
//    writer = std::make_unique<FileWriter>();
//    // Check for valid status from FileWriter
////		if () {
////
////		}
//    data_packet = writer->getEmptyBufferForPixel(current_pix);
//    timer_interval = writer->params_.getDwellTime() * 1000;

//    /* Initialize semaphore for process synchronization */
//    sem_probe = sem_open("/daq_probe", 0);
//    sem_reply = sem_open("/daq_reply", 0);
//  }


//  configTimer();
//  ret = CAEN_DGTZ_SWStartAcquisition(handle[0]);
//  if (ret || !daq_mode) {
//    printf("[!] Problem starting acquisition, error code: %d\n", ret);
//  }
//  else {
//    printf("[!] Acquisition started\n");
//  }

//  while (daq_enable)
//  {
//    if (flag_newpix)
//    {
//      // Cost of calling these two functions is ~ 10ms
//      writer->pushToQueue(std::move(data_packet));
//      data_packet = writer->getEmptyBufferForPixel(++current_pix);

//      if (current_pix / writer->params_.getPixels()) {
//        printf("[!] End of scan\n");
//        sem_post(sem_reply);
//        sem_close(sem_reply);
//        sem_close(sem_probe);
//        daq_enable = false;
//        continue;
//      }

//      if ( !(current_pix % writer->params_.getXDim()) )
//      {
//        // Sync new line
//        stopTimer();
//        CAEN_DGTZ_SWStopAcquisition(handle[0]);
//        configTimer();
//        ret = CAEN_DGTZ_SWStartAcquisition(handle[0]);
//        if (ret) {
//          printf("[!] Can't start acquisition, error: %d\n", ret);
//        }

//      }
//      flag_newpix = false;
//    }
//    readDataFromDGTZ();
//  }

//  daq_stopped = true;
//  cleanup();
}

}  // namespace maxrfd::daq





