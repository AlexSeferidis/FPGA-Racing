import AWS from 'aws-sdk';

const Cognito = new AWS.CognitoIdentityServiceProvider({ region: 'us-east-1' });

export const handler = async (event) => {
  if (
    event.userName &&
    event.userPoolId &&
    event.request &&
    event.request.userAttributes &&
    event.request.userAttributes.email
  ) {
    const cognitoRequestParams = {
      UserPoolId: event.userPoolId,
      Filter: `email = "${event.request.userAttributes.email}"`
    };

    try {
      const data = await Cognito.listUsers(cognitoRequestParams).promise();

      if (
        data.Users.length > 0 &&
        data.Users[0].Username.toLowerCase() !== event.userName.toLowerCase()
      ) {
        throw new Error('An account already exists for that email');
      }
    } catch (err) {
      throw err;
    }
  }

  return event;
};