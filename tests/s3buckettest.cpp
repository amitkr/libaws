#include <iostream>
#include <libaws/aws.h>

using namespace aws;

int
createbuckettest(S3Connection* lS3Rest)
{
  {
    try {
      CreateBucketResponsePtr lCreateBucket = lS3Rest->createBucket("28msec_s3buckettest");
  		std::cout << "Bucket created successfully" << std::endl;
      std::cout << "  Location: " << lCreateBucket->getLocation() << std::endl;
      std::cout << "  Date: " << lCreateBucket->getDate() << std::endl;
      if (!lCreateBucket->getETag().empty()) 
        std::cout << "  ETag: " << lCreateBucket->getETag() << std::endl;
      std::cout << "  x-amz-id-2: " << lCreateBucket->getAmazonId() << std::endl;
      std::cout << "  x-amz-request-id: " << lCreateBucket->getRequestId() << std::endl;
    } catch (CreateBucketException& e) {
  		std::cerr << "Couldn't create bucket" << std::endl;
  		std::cerr << e.what() << std::endl;
      return 1;
    }
  }

  {
    // must fail
    try {
      CreateBucketResponsePtr lCreateBucket = lS3Rest->createBucket("");
  		std::cout << "Bucket created successfully" << std::endl;
      return 1;
    } catch (CreateBucketException& e) {
  		std::cerr << "Couldn't create bucket" << std::endl;
  		std::cerr << e.what() << std::endl;
    }
  }
  return 0;
}

int
listallbuckets(S3Connection* lS3Rest)
{
  {
    try {
      ListAllBucketsResponsePtr lListAllBuckets = lS3Rest->listAllBuckets();
  		std::cout << "Buckets listed successfully" << std::endl;
      std::cout << "  Date: " << lListAllBuckets->getDate() << std::endl;
      if (!lListAllBuckets->getETag().empty()) 
        std::cout << "  ETag: " << lListAllBuckets->getETag() << std::endl;
      std::cout << "  x-amz-id-2: " << lListAllBuckets->getAmazonId() << std::endl;
      std::cout << "  x-amz-request-id: " << lListAllBuckets->getRequestId() << std::endl;

      std::cout << "OwnerId " << lListAllBuckets->getOwnerId() << std::endl;
      std::cout << "OwnerDisplayName " << lListAllBuckets->getOwnerDisplayName() << std::endl;

      ListAllBucketsResponse::Bucket lBucket;
      lListAllBuckets->open();
      while (lListAllBuckets->next(lBucket)) {
        std::cout << "BucketName " << lBucket.BucketName << std::endl;
        std::cout << "CreationDate " << lBucket.CreationDate << std::endl;
      }

      lListAllBuckets->close();

    } catch (CreateBucketException& e) {
  		std::cerr << "Couldn't list buckets" << std::endl;
  		std::cerr << e.what() << std::endl;
      return 1;
    }
  }
  return 0;
}

int
listbuckettest(S3Connection* lS3Rest)
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

int deletebuckettest(S3Connection* lS3Rest)
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

  {
    // must fail
    try {
      DeleteBucketResponsePtr lDeleteBucket = lS3Rest->deleteBucket("");
      std::cout << "Bucket created successfully" << std::endl;
      return 1;
    } catch (DeleteBucketException& e) {
      std::cerr << "Couldn't delete bucket" << std::endl;
      std::cerr << e.what() << std::endl;
    }
  }
  return 0;
}


int
s3buckettest(int argc, char** argv) 
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
    lReturnCode = createbuckettest(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

#if 0
    lReturnCode = listallbuckets(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

    lReturnCode = listbuckettest(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;

    lReturnCode = deletebuckettest(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;
#endif

  } catch (AWSConnectionException& e) {
    std::cerr << e.what() << std::endl;
    return 2;
  }

  lFactory->shutdown();

  return 0;
}
