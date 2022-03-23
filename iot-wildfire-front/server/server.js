const express = require('express')
const path = require('path')
const cors = require('cors')
//const fs = require('fs')
const https = require('https')

const app = express()

const PORT = 8080
const HOST = '0.0.0.0'

//For a https server certification
// const key = fs.readFileSync('server/keys/key.pem')
// const certificate = fs.readFileSync('server/keys/cert.pem')
// let options = {
//     key: key,
//     cert:certificate
// }

const {json, urlencoded} = express

//basic parse configuration
app.use(json())
app.use(urlencoded({extended:false}))
app.use(cors())

//start https server
//let server = https.create(options,app)
//server.listen(PORT,()=>{console.log(`Server on port: ${PORT} and host ${HOST}`)})

app.listen(PORT,HOST,()=>{console.log(`Server on port: ${PORT} and host ${HOST}`)});