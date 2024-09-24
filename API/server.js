const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

// Middleware
app.use(bodyParser.json());

// MongoDB connection
mongoose.connect('mongodb+srv://admin:ztUys0KA2QDY6Pp4@cluster0.225dn.mongodb.net/test', {
    useNewUrlParser: true,
    useUnifiedTopology: true,
});

// Create a schema and model for the data  
const DataSchema = new mongoose.Schema({
    smoke_detector: Number,
    pm1: Number,
    pm2_5: Number,
    pm10: Number,
    temperature: Number,
    humidity: Number,  

    
    co2: Number, 
    ammonia: Number, 
    gas: Number,  
    timestamp: {
        type: Date,
        default: Date.now,
    },
});

const Data = mongoose.model('Data', DataSchema, 'value');    //ชื่อ collection

// POST route to receive data from ESP32
app.post('/api/data', (req, res) => {
    //const { smoke_detector, pm1, pm2_5, pm10, temperature, humidity, co2, ammonia, gas  } = req.body;
    const { smoke_detector, pm1, pm2_5, pm10 } = req.body;
    
    //const data = new Data({ smoke_detector, pm1, pm2_5, pm10, temperature, humidity, co2, ammonia, gas });
    const data = new Data({ smoke_detector, pm1, pm2_5, pm10 });
                            
    data.save()
        .then(() => res.status(201).send('Data saved successfully'))
        .catch(err => res.status(400).send('Error saving data'));
});

// Start the server
app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
