const router = require('express').Router()
const fs = require('fs')

//TODO returns the current temperature for a sensor given its id
// router.get('/show')

//TODO returns all of the stored temperatures for a sensor given its id
// router.get('/history')

//Register a new temperature for a sensor given its id
//FIXME Use DynamoDB instead of the db.json file
router.post('/register',(req,res)=>{
    try{
        //verify body contents
        console.log(req.body)
        let {sensorId, temperature} = req.body
        if(sensorId !== undefined && temperature !== undefined){

            //read the db.json and parse it to a js object
            let dbJSON = JSON.parse(fs.readFileSync('server/db.json'))

            //create the new data to store in the db.json
            let data = {
                uid:dbJSON.length == 0 ? 1 : Math.max.apply(Math,dbJSON.map(function(o){return o.uid;})) + 1,
                sensorId: sensorId,
                temperature: temperature,
                dateTime: +new Date()
            }

            //append the date to the db.json
            dbJSON.push(data)
            
            //save the new version of the db by overwritting the old db.json
            fs.writeFileSync('server/db.json',JSON.stringify(dbJSON))

            res.status(200).send({'Success':'Temperature registered'})

        } else {
            res.status(400).send({'Error while registering the temperature':"Missing parameters -sensorId or temperature-"})
        }

    } catch (error){
        res.status(400).send({'Error while registering the temperature':error})
    }
})

//TODO Delete the last registered temperature for a sensor given its id
// router.delete('/delete')



module.exports = router
