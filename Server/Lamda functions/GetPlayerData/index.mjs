import AWS from 'aws-sdk';

const Cognito = new AWS.CognitoIdentityServiceProvider({ region: 'us-east-1' });
const DynamoDb = new AWS.DynamoDB({ region: 'us-east-1' });

export const handler = async (event) => {
  let response;
  let raisedError;
  let accessToken;

  if (event.headers && event.headers['Authorization']) {
    accessToken = event.headers['Authorization'];
  }

  const cognitoRequestParams = {
    AccessToken: accessToken
  };

  let sub;

  try {
    const userData = await Cognito.getUser(cognitoRequestParams).promise();

    if (userData && userData.UserAttributes) {
      for (const attribute of userData.UserAttributes) {
        if (attribute.Name === 'sub') {
          sub = attribute.Value;
          break;
        }
      }
    }
  } catch (err) {
    raisedError = err;
  }

  if (raisedError) {
    response = {
      statusCode: 400,
      body: JSON.stringify({
        error: raisedError.message
      })
    };
    return response;
  }

  const dynamoDbRequestParams = {
    TableName: 'Players',
    Key: {
      Id: { S: sub }
    }
  };

  let playerData;

  try {
    const data = await DynamoDb.getItem(dynamoDbRequestParams).promise();

    if (data && data.Item) {
      playerData = data.Item;
    }

    response = {
      statusCode: 200,
      body: JSON.stringify({
        playerData: playerData
      })
    };
  } catch (err) {
    response = {
      statusCode: 400,
      body: JSON.stringify({
        error: err.message
      })
    };
  }

  return response;
};
