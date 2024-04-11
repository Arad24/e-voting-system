function nodesVectorToString(nodes) {
    let result = "[";

    for (let i = 0; i < nodes.length; ++i) {
        result += nodes[i];
        if (i < nodes.length - 1) {
            result += ", ";
        }
    }

    result += "]";
    return result;
}

function nodesStringToVector(nodesString) {
    const nodes = [];

    const start = nodesString.indexOf("[");
    const end = nodesString.indexOf("]");

    if (start !== -1 && end !== -1) {
        const content = nodesString.substring(start + 1, end);
        const tokens = content.split(",");

        for (const token of tokens) {
            const node = token.trim();
            nodes.push(node);
        }
    }

    return nodes;
}

function isCorrectFormat(nodesString) {
    if (nodesString.length < 2 || nodesString[0] !== '[' || nodesString[nodesString.length - 1] !== ']') {
        return false;
    }

    const content = nodesString.substring(1, nodesString.length - 1).trim();
    if (content.length === 0) {
        return false;
    }

    const tokens = content.split(",");
    for (const token of tokens) {
        const trimmedToken = token.trim();
        const parts = trimmedToken.split(":");
        if (parts.length !== 2 || parts[0].trim().length === 0 || parts[1].trim().length === 0 || isNaN(Number(parts[1]))) {
            return false;
        }
    }

    return true;
}

module.exports = { nodesVectorToString, nodesStringToVector, isCorrectFormat };
