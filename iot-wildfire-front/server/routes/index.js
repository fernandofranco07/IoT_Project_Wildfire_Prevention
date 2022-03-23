const router = require('express').Router()

const temperatureRoute = require('./temperature')

router.use('/api/temperature',temperatureRoute)

//Test get route
router.get('/api/test',(req,res)=>{
    res.send({'Test':'Hello world'})
})

module.exports = router