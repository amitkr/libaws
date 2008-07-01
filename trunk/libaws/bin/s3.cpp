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
#include <iostream>
#include <fstream>
#include <libaws/aws.h>

using namespace aws;

bool
listBuckets ( S3ConnectionPtr aS3 )
{
  try {
    ListAllBucketsResponsePtr lRes = aS3->listAllBuckets();
    std::cout << "buckets listed sucessfully" << std::endl;
    ListAllBucketsResponse::Bucket lBucket;
    lRes->open();
    while (lRes->next(lBucket)) {
      std::cout << "   Bucket Name: " << lBucket.BucketName
                << " | Creation Date: " << lBucket.CreationDate << "\n";
    }
    lRes->close();
  } catch (CreateBucketException &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool
listBucket(S3ConnectionPtr aS3, std::string aBucketName, std::string aPrefix, 
           std::string aMarker, std::string aDelimiter, int aMaxKeys) {
  ListBucketResponsePtr lListBucket;
  ListBucketResponse::Object lObject;

  std::string lMarker;
  try {
    do
    {
      lListBucket = aS3->listBucket(aBucketName, aPrefix, lMarker, aDelimiter, aMaxKeys);
      lListBucket->open();
      while (lListBucket->next(lObject)) {
        std::cout << "   Key: " << lObject.KeyValue << " | Last Modified: " << lObject.LastModified;
        std::cout <<  " | ETag: " << lObject.ETag << " | Size: " << lObject.Size << std::endl;
        lMarker = lObject.KeyValue;
      }
      lListBucket->close();
      std::vector<std::string> lCommonPrefixes = lListBucket->getCommonPrefixes();
      for (std::vector<std::string>::const_iterator lIter = lCommonPrefixes.begin();
           lIter != lCommonPrefixes.end(); ++lIter) {
        std::cout << "CommonPrefix " << *lIter << std::endl;
      }
    } while (lListBucket->isTruncated());
  } catch (ListBucketException &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool deleteAllEntries(S3ConnectionPtr aS3, std::string aBucketName)
{
  ListBucketResponsePtr lListBucket;
  ListBucketResponse::Object lObject;
  std::string lMarker;
  try {
    do
    {
      lListBucket = aS3->listBucket(aBucketName, "", lMarker);
      lListBucket->open();
      while (lListBucket->next(lObject)) {
        DeleteResponsePtr lDelete = aS3->del(aBucketName, lObject.KeyValue);
        std::cout << "deleted item with key " << lObject.KeyValue << std::endl;
        lMarker = lObject.KeyValue;
      }
      lListBucket->close();
    } while (lListBucket->isTruncated());
  } catch (ListBucketException &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool createBucket( S3ConnectionPtr aS3, const std::string& aBucketName) {
  try {
    CreateBucketResponsePtr lRes = aS3->createBucket(aBucketName);
    std::cout << "bucket created successfully" << std::endl;
    std::cout << "   bucket-name: "  << lRes->getBucketName() << std::endl;
  } catch (CreateBucketException &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool deleteBucket ( S3ConnectionPtr aS3, const std::string& aBucketName) {
  try {
    DeleteBucketResponsePtr lRes = aS3->deleteBucket(aBucketName);
    std::cout << "bucket deleted sucessfully" << std::endl;
    std::cout << "   bucket-name: " << lRes->getBucketName() << std::endl;
  } catch (DeleteBucketException &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool put ( S3ConnectionPtr aS3, const std::string& aBucketName, const std::string aFileName, const std::string aKey )
{
  try {
    std::ifstream lInStream(aFileName.c_str());
    if (!lInStream) {
      std::cerr << "file not found or accessible: " << aFileName << std::endl;
      return false;
    }
    PutResponsePtr lPut = aS3->put(aBucketName, aKey.length()==0?aFileName:aKey, lInStream, "text/plain");
  } catch (PutException &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

int
main ( int argc, char** argv )
{
  char* lBucketName = 0;
  char* lPrefix = 0;
  char* lMarker = 0;
  char* lDelimiter = 0;
  int   lMaxKeys = 0;
  char* lAction = 0;
  char* lAccessKeyId = 0;
  char* lSecretAccessKey = 0;
  char* lFileName = 0;
  char* lKey = 0;
  int c;
  opterr = 0;

  AWSConnectionFactory* lFactory = AWSConnectionFactory::getInstance();

  while ((c = getopt (argc, argv, "hik:a:n:f:p:mx:d:s:")) != -1)
    switch (c)
    {
      case 'i':
        lAccessKeyId = optarg;
        break;
      case 's':
        lSecretAccessKey = optarg;
        break;
      case 'a':
        lAction = optarg;
        break;
      case 'n':
        lBucketName = optarg;
        break;
      case 'p':
       lPrefix = optarg;
        break;
      case 'm':
        lMarker = optarg;
        break;
      case 'd':
        lDelimiter = optarg;
        break;
      case 'x':
        lMaxKeys = atoi(optarg);
        break;
      case 'f':
        lFileName = optarg;
        break;
      case 'k':
        lKey = optarg;
        break;
      case 'h': {
        std::cout << "libaws version " << lFactory->getVersion() << std::endl;
        std::cout << "Usage: s3 <options>" << std::endl;
        std::cout << "  -i: AWS Access Key Id"  << std::endl;
        std::cout << "  -s: AWS Secret Access Key"  << std::endl;
        std::cout << "  -a action: action to perform" << std::endl;
        std::cout << "             list: list all buckets" << std::endl;
        std::cout << "             create: create a bucket" << std::endl;
        std::cout << "             delete: delete a bucket" << std::endl;
        std::cout << "             entries: list all objects in a bucket" << std::endl;
        std::cout << "             delete-all-entries: delete all entries in a bucket" << std::endl;
        std::cout << "             put: put a file on s3" << std::endl;
        std::cout << "  -n name: name of bucket"  << std::endl;
        std::cout << "  -p prefix: prefix for entries to list "  << std::endl;
        std::cout << "  -m marker: marker for entries to list"  << std::endl;
        std::cout << "  -d delimiter: delimiter of keys to list" << std::endl;
        std::cout << "  -x maxkeys: maximum number of keys to list" << std::endl;
        std::cout << "  -k key: key of the object" << std::endl;
        exit(1);
      }
      case '?':
        if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
              "Unknown option character `\\x%x'.\n",
              optopt);
        exit(1);
      default:
        exit(1);
    }
  
  if (!lAccessKeyId)
    lAccessKeyId = getenv("AWS_ACCESS_KEY");

  if (!lSecretAccessKey)
    lSecretAccessKey = getenv("AWS_SECRET_ACCESS_KEY");

  if (!lAccessKeyId) {
    std::cerr << "No Access Key given" << std::endl;
    std::cerr << "Either use -i as a command line argument or set AWS_ACCESS_KEY as an environmental variable" << std::endl;
    exit(1);
  }
  if (!lSecretAccessKey) {
    std::cerr << "No Secret Access Key given" << std::endl;
    std::cerr << "Either use -s as a command line argument or set AWS_SECRET_ACCESS_KEY as an environmental variable" << std::endl;
    exit(1);
  }


  S3ConnectionPtr lS3Rest =  lFactory->createS3Connection(lAccessKeyId, lSecretAccessKey);

  if (!lAction) {
    std::cerr << "No Action parameter specified." << std::endl;
    std::cerr << "Use -a as a command line argument" << std::endl;
    exit(1);
  }
  std::string lActionString(lAction);

  if ( lActionString.compare ( "list" ) == 0 ) {
    listBuckets(lS3Rest);
  } else if ( lActionString.compare ( "create" ) == 0 ) {
    if (!lBucketName) {
      std::cerr << "No bucket name parameter specified." << std::endl;
      std::cerr << "Use -n as a command line argument" << std::endl;
      exit(1);
    }
    createBucket(lS3Rest, lBucketName);
  } else if ( lActionString.compare ( "delete" ) == 0) {
    if (!lBucketName) {
      std::cerr << "No bucket name parameter specified." << std::endl;
      std::cerr << "Use -n as a command line argument" << std::endl;
      exit(1);
    }
    deleteBucket(lS3Rest, lBucketName);
  } else if ( lActionString.compare ( "entries" ) == 0) {
    if (!lBucketName) {
      std::cerr << "No bucket name parameter specified." << std::endl;
      std::cerr << "Use -n as a command line argument" << std::endl;
      exit(1);
    }
    listBucket(lS3Rest, lBucketName, lPrefix==0?"":lPrefix, lMarker==0?"":lMarker,  
               lDelimiter==0?"":lDelimiter, lMaxKeys==0?-1:lMaxKeys);
  } else if ( lActionString.compare ( "delete-all-entries" ) == 0) {
    if (!lBucketName) {
      std::cerr << "No bucket name parameter specified." << std::endl;
      std::cerr << "Use -n as a command line argument" << std::endl;
      exit(1);
    }
    deleteAllEntries(lS3Rest, lBucketName);
  } else if ( lActionString.compare ( "put" ) == 0) {
    if (!lBucketName) {
      std::cerr << "No bucket name parameter specified." << std::endl;
      std::cerr << "Use -n as a command line argument" << std::endl;
      exit(1);
    }
    if (!lFileName) {
      std::cerr << "No file specified." << std::endl;
      std::cerr << "Use -f as a command line argument" << std::endl;
      exit(1);
    }
    put(lS3Rest, lBucketName, lFileName, lKey==0?"":lKey);
  }

}
