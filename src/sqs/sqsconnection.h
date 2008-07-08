/*
 * Copyright 2008 28msec, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef AWS_SQS_SQSCONNECTION_H
#define AWS_SQS_SQSCONNECTION_H

#include "common.h"

#include <map>
#include <iostream>

#include "awsconnection.h"

namespace aws {


  namespace sqs {

    class SQSConnection : public AWSQueryConnection
    {
      public:
        static const std::string DEFAULT_VERSION;
        static const std::string DEFAULT_HOST;
        
      
      public:
        SQSConnection(const std::string& aAccessKeyId, 
                      const std::string& aSecretAccessKey);
      
        CreateQueueResponsePtr
        createQueue ( const std::string &aQueueName, int aDefaultVisibilityTimeout );

        /*
        DeleteQueueResponsePtr
        deleteQueue ( const std::string &aQueueUrl, bool aForceDeletion = false );

        ListQueuesResponsePtr
        listQueues ( const std::string &aQueueNamePrefix = "" );

        SendMessageResponsePtr
        sendMessage ( const std::string &aQueueName, const char* aContent, size_t aContentSize );

        SendMessageResponsePtr
        sendMessage ( const std::string &aQueueName, const std::string &aContent );

        PeekMessageResponsePtr
        peekMessage ( const std::string &aQueueName, const std::string &aMessageId );

        ReceiveMessageResponsePtr
        receiveMessage ( const std::string &aQueueName,
                         int aNumberOfMessages = -1,
                         int aVisibilityTimeout = -1 );

        DeleteMessageResponsePtr
        deleteMessage ( const std::string &aQueueName, const std::string &aMessageId );*/

    };

  } /* namespace sqs  */
} /* namespace aws */

#endif        /* !AWS_SQS_SQSCONNECTION_H */
