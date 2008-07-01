#include <iostream>
#include <sstream>
#include <libaws/aws.h>

using namespace aws;

int
createbucket(S3Connection* lS3Rest)
{
  {
    try {
      CreateBucketResponsePtr lCreateBucket = lS3Rest->createBucket("28msec_s3buckettest");
  		std::cout << "Bucket created successfully" << std::endl;
    } catch (CreateBucketException& e) {
  		std::cerr << "Couldn't create bucket" << std::endl;
  		std::cerr << e.what() << std::endl;
      return 1;
    }
  }
  return 0;
}

int
put(S3Connection* lS3Rest)
{
  {
    try {
      std::istringstream lStream("This is a test!");

      PutResponsePtr lPut = lS3Rest->put("28msec_s3buckettest", "a/b/c",
                                         lStream, "text/plain");
      std::cout << "Object sent successfully" << std::endl;
    } catch (PutException& e) {
  		std::cerr << "Couldn't put object" << std::endl;
  		std::cerr << e.what() << std::endl;
      return 1;
    }
  }

  {
    try {
      char msg[5];
      msg[0] = 'H'; msg[1] = 'e'; msg[2] = 'l'; msg[3] = 'l'; msg[4] = 'o';
      PutResponsePtr lPut = lS3Rest->put("28msec_s3buckettest", "a/b/c/d",
                                         msg, "text/plain", 5);
      std::cout << "Object sent successfully" << std::endl;
    } catch (PutException& e) {
  		std::cerr << "Couldn't put object" << std::endl;
  		std::cerr << e.what() << std::endl;
      return 1;
    }
  }
  return 0;
}

int
listbucket(S3Connection* lS3Rest)
{
  {
    try {
      ListBucketResponsePtr lListBucket = lS3Rest->listBucket("28msec_s3buckettest", "", "", 10);
      std::cout << "Listing Buckets:" << std::endl;
      ListBucketResponse::Object lObject;
      lListBucket->open();
      while (lListBucket->next(lObject)) {
        std::cout << "   Key: " << lObject.KeyValue << " | Last Modified: " << lObject.LastModified;
        std::cout <<  " | ETag: " << lObject.ETag << " | Length: " << lObject.Size << std::endl;
      }
      lListBucket->close();

    } catch (ListBucketException& e) {
      std::cerr << "Couldn't list bucket" << std::endl;
      std::cerr << e.what() << std::endl;
      return 4;
    }
  }
  return 0;
}	

int
getobject(S3Connection* lS3Rest)
{
  {
    try {
      GetResponsePtr lGet = lS3Rest->get("28msec_s3buckettest", "a/b/c");

      std::istream& lInStream = lGet->getInputStream();

      char lBuf[20];
      size_t lRead = lInStream.readsome(lBuf, 20);
      lBuf[lRead] = 0;

      std::cout.write (lBuf, lRead);

      std::cout << std::endl << "Object retrieved successfully" << std::endl;

      std::cout << "ETag " << lGet->getETag() << std::endl;
    
    } catch (GetException& e) {
      std::cerr << "Could get object" << std::endl;
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }

  {
    try {
      lS3Rest->get("28msec_s3buckettest", "x");
      return 1;
    } catch (GetException& e) {
      std::cerr << "Couldn't get object" << std::endl;
      std::cerr << e.what() << std::endl;
    }
  }
  return  0;
}

int
deleteobject(S3Connection* lS3Rest)
{
  {
    try {
      lS3Rest->del("28msec_s3buckettest", "a/b/c");
      lS3Rest->del("28msec_s3buckettest", "a/b/c/d");
      std::cout << "Object deleted successfully" << std::endl;
    } catch (DeleteException& e) {
  		std::cerr << "Couldn't delete object" << std::endl;
  		std::cerr << e.what() << std::endl;
      return 1;
    }
  }
  return 0;
}

int deletebucket(S3Connection* lS3Rest)
{
	{
    try {
      DeleteBucketResponsePtr lDeleteBucket = lS3Rest->deleteBucket("28msec_s3buckettest");
      std::cout << "Bucket delete successfully" << std::endl;
		} catch (DeleteBucketException& e) {
			std::cerr << "Couldn't delete bucket" << std::endl;
			std::cerr << e.what() << std::endl;
			return 3;
		}
	}
  return 0;
}


int
s3objecttest(int argc, char** argv) 
{
	
	AWSConnectionFactory* lFactory = AWSConnectionFactory::getInstance();

  std::cout << "Testing libaws version " << lFactory->getVersion() << std::endl;

  char* lAccessKeyId = getenv("AWS_ACCESS_KEY");
  char* lSecretAccessKey = getenv("AWS_SECRET_ACCESS_KEY");

  if (lAccessKeyId == 0 || lSecretAccessKey == 0) {
    std::cerr << "Environment variables (i.e. AWS_ACCESS_KEY or AWS_SECRET_ACCESS_KEY) not set" 
              << std::endl;
    return 1;
  }
	
  S3ConnectionPtr lS3Rest = lFactory->createS3Connection(lAccessKeyId, lSecretAccessKey);

  int lReturnCode;
  try {
    lReturnCode = createbucket(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

    lReturnCode = put(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

    lReturnCode = listbucket(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

    lReturnCode = getobject(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

    lReturnCode = deleteobject(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

    lReturnCode = deletebucket(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

  } catch (AWSConnectionException& e) {
    std::cerr << e.what() << std::endl;
    return 2;
  }

  lFactory->shutdown();

  return 0;
}
