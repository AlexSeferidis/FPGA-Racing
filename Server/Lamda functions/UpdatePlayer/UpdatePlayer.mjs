import AWS from 'aws-sdk';

const DynamoDb = new AWS.DynamoDB({ region: 'us-east-1' });

export const handler = async (event) => {
  const playerId = event.queryStringParameters.playerId;

  if (!playerId) {
    return {
      statusCode: 400,
      body: JSON.stringify({ error: 'PlayerId is required in the query parameters.' }),
    };
  }

  const position = event.queryStringParameters.position;

  if (position === undefined || position < 1 || position > 12) {
    return {
      statusCode: 400,
      body: JSON.stringify({ error: 'Invalid position value. It should be between 1 and 12.' }),
    };
  }

  try {
    // Retrieve existing player data
    const getPlayerParams = {
      TableName: 'Players',
      Key: {
        Id: { S: playerId },
      },
    };

    const existingPlayer = await DynamoDb.getItem(getPlayerParams).promise();

    if (!existingPlayer.Item) {
      return {
        statusCode: 404,
        body: JSON.stringify({ error: 'Player not found.' }),
      };
    }

    // Update the PositionCounts array
    const positionCounts = existingPlayer.Item.PositionCounts.L;
    positionCounts[position - 1].N = (parseInt(positionCounts[position - 1].N) + 1).toString();

    // Update DynamoDB with the new data
    const updatePlayerParams = {
      TableName: 'Players',
      Key: {
        Id: { S: playerId },
      },
      UpdateExpression: 'SET PositionCounts = :positionCounts',
      ExpressionAttributeValues: {
        ':positionCounts': { L: positionCounts },
      },
    };

    await DynamoDb.updateItem(updatePlayerParams).promise();

    return {
      statusCode: 200,
      body: JSON.stringify({ success: 'Player data updated successfully.' }),
    };
  } catch (err) {
    console.error(err);
    return {
      statusCode: 500,
      body: JSON.stringify({ error: 'Internal server error.' }),
    };
  }
};
