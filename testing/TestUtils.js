var __testResults = []
var __request
var __response

function test(testName, testFunc) {
    try {
        testFunc(__request, __response);
        __testResults.push({testName, succeeded: true})
    } catch (e) {
        __testResults.push({testName, succeeded: false})
    }
}

function assert(value) {
    if (!value)
        throw 'Assertion failed.'
}
