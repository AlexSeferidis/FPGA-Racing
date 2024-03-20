import AWS from 'aws-sdk';

const DynamoDb = new AWS.DynamoDB({ region: 'us-east-1' });

export const handler = async (event) => {
  if (event.triggerSource && event.triggerSource === 'PostConfirmation_ConfirmSignUp') {
    if (event.request && event.request.userAttributes && event.request.userAttributes.sub) {
      const playerId = event.request.userAttributes.sub;

      const initialPositionCounts = Array(12).fill(0); // Initialize with 12 positions, each set to 0

      const dynamoDbRequestParams = {
        TableName: 'Players',
        Item: {
          Id: { S: playerId },
          Wins: { N: '0' },
          Losses: { N: '0' },
          PositionCounts: { L: initialPositionCounts.map(count => ({ N: count.toString() })) }, // Convert each count to a number
        }
      };

      try {
        await DynamoDb.putItem(dynamoDbRequestParams).promise();
      } catch (err) {
        throw err;
      }
    }
  }

  return event;
};