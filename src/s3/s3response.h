#ifndef AWS_S3_S3RESPONSE_H
#define AWS_S3_S3RESPONSE_H

#include "common.h"

#include <libaws/awstime.h>
#include <libaws/s3exception.h>
#include <vector>
#include <time.h>
#include <sstream>
#include <istream>

#include "response.h"

namespace aws { namespace s3  {

  class CurlStreamBuffer;

  class S3ResponseError
  {
    friend class CreateBucketHandler;
    friend class DeleteBucketHandler;
    friend class ListAllBucketsHandler;
    friend class ListBucketHandler;
    friend class GetHandler;
    friend class PutHandler;
    friend class HeadHandler;
    friend class DeleteHandler;
    friend class S3Connection;
    friend class S3Response;

  private:
    static S3Exception::ErrorCode
    parseError(const std::string&);

    S3ResponseError();

  public:
    S3Exception::ErrorCode
    getErrorCode() const { return theErrorCode; }

    const std::string&
    getErrorMessage() const { return theErrorMessage; }

    const std::string&
    getRequestId() const { return theRequestId; }

    const std::string&
    getHostId() const { return theHostId; }

  protected:
    S3Exception::ErrorCode   theErrorCode;
    std::string              theErrorMessage;
    std::string              theRequestId;
    std::string              theHostId;
  };


  class S3Response : public aws::Response 
  {
    friend class S3Connection;

  public:
    S3Response();
    virtual ~S3Response();

    S3ResponseError&
    getS3ResponseError();

    const std::string&
    getRequestId() const { return theRequestId; }

    const std::string&
    getETag() const      { return theETag; }

    const std::string&
    getDate() const      { return theDate; }

    const std::string&
    getAmazonId() const  { return theAmazonId; }

  protected:
    S3ResponseError theS3ResponseError;
    std::string     theRequestId;
    std::string     theETag;
    std::string     theDate;
    std::string     theAmazonId;
  };

class CreateBucketResponse : public S3Response
{
	friend class CreateBucketHandler;
  friend class S3Connection;
	
private: // only a S3Connection can create me
  CreateBucketResponse(const std::string& aBucketName);

public:
  virtual ~CreateBucketResponse();
	
  //! The name of the bucket that was created
	const std::string&
  getBucketName() { return theBucketName; }

  const std::string&
  getLocation()   { return theLocation; }


protected:
  std::string theBucketName;
  std::string theLocation;
};

class ListAllBucketsResponse : public S3Response
{
	friend class ListAllBucketsHandler;
  friend class S3Connection;
	
public:
	struct Bucket {
		std::string BucketName;
		std::string CreationDate;
	};

public:
  ListAllBucketsResponse();
  virtual ~ListAllBucketsResponse();

  void
  open() const;

  bool
  next(Bucket&) const;

  void
  close() const;

  const std::string&
  getOwnerId() const;

  const std::string&
  getOwnerDisplayName() const;

protected:
	std::string theOwnerId;
	std::string theOwnerDisplayName;
	std::vector<Bucket> theBuckets;
  mutable std::vector<Bucket>::const_iterator theIterator;
};

class ListBucketResponse : public S3Response
{
  friend class ListBucketHandler;
  friend class S3Connection;
    
public:
    struct Key {
      std::string KeyValue;
      std::string LastModified;
      std::string ETag;
      intmax_t    Length;
    };
    
public:
    ListBucketResponse(const std::string& aBucketName, const std::string& aPrefix,
                       const std::string& aMarker, int aMaxKeys);
    virtual ~ListBucketResponse();
    
    virtual void
    open() const;

    virtual bool
    next(Key& aKey) const;

    virtual void
    close() const;
    
    const std::string&
    getBucketName() { return theBucketName; }

    const std::string&
    getPrefix() { return thePrefix; }

    const std::string&
    getMarker() { return theMarker; }

    const std::string&
    getDelimiter() { return theDelimiter; }

    const std::vector<std::string>&
    getCommonPrefixes() { return theCommonPrefixes; }

    int
    getMaxKeys() { return theMaxKeys; }

    bool
    isTruncated() { return theIsTruncated; }

protected:
    std::string                              theBucketName;
    std::string                              thePrefix;
    std::string                              theMarker;
    std::string                              theDelimiter;
    int                                      theMaxKeys;
    bool                                     theIsTruncated;
    std::vector<Key>                         theKeys;
    std::vector<std::string>                 theCommonPrefixes;
    mutable std::vector<Key>::const_iterator theIterator;
};


class DeleteBucketResponse : public S3Response
{
  friend class DeleteBucketHandler;
  friend class S3Connection;
    
public:
    DeleteBucketResponse(const std::string& aBucketName);
    virtual ~DeleteBucketResponse();
    
	  const std::string&
    getBucketName() const { return theBucketName; }

public:
    std::string             theBucketName;
};

class PutResponse : public S3Response
{
    friend class PutHandler;
  friend class S3Connection;

public:
    PutResponse(const std::string& aBucketName);    
    virtual ~PutResponse();
    
public:
    const std::string&
    getBucketName() const { return theBucketName; }

protected:
    std::string theBucketName;
    
};

class GetResponse : public S3Response
{
  friend class GetHandler;
  friend class S3Connection;

public:
    GetResponse(const std::string& aBucketName, const std::string& aKey);
    virtual ~GetResponse();
    
public:
    const std::string&
    getKey() const { return theKey; }

    const std::string&
    getBucketName() const { return theBucketName; }
  
    std::istream&
    getInputStream() const {  return *theInputStream; }

    const Time&
    getLastModified() const { return theLastModified; }

    bool
    isModified() const { return theIsModified; }
    
protected:
    std::string       theBucketName;
    std::string       theKey;
    size_t            theContentSize;
    CurlStreamBuffer* theStreamBuffer;
    std::istream*     theInputStream;
    Time              theLastModified;
    bool              theIsModified;
};

class HeadResponse : public S3Response
{
    friend class HeadHandler;
public:
    HeadResponse(const std::string& aBucketName);
    virtual ~HeadResponse();
    
    const std::string&
    getBucketName() const { return theBucketName; }

protected:
    std::string theBucketName;
};

class DeleteResponse : public S3Response
{
    friend class DeleteHandler;
    
public:
    DeleteResponse(const std::string& aBucketName, const std::string& aKey);
    virtual ~DeleteResponse();
    
    const std::string&
    getBucketName() const { return theBucketName; }

    const std::string&
    getKey() const { return theKey; }
        
protected:
    std::string     theBucketName;
    std::string     theKey;
};
    
} } // end namespaces

#endif