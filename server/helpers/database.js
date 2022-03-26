//library for reading and writing in the db
//TODO Refactor this to use DynamoDB

let fs = require('fs')

let pathDB = "server/db.json"

//container for the functions
let lib = {}

//return parsed data as an object in the database 
lib.read = function(){
    return JSON.parse(fs.readFileSync(pathDB))
}

//write data to the database
lib.write = function(data){
    try{
        fs.writeFileSync(pathDB,JSON.stringify(data))
    }catch(error){
        console.log({"There was an error writing to the DB":error})
        return false
    }
} 

module.exports = lib