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

/*const DataSchema = new mongoose.Schema({
    temperature: String,
    humidity: String,
    timestamp: {
        type: Date,
        default: Date.now,
    },
});   */                                      

const DataSchema = new mongoose.Schema({
    pm1: String,
    pm2_5: String,
    pm10: String,
    timestamp: {
        type: Date,
        default: Date.now,
    },
});  

const Data = mongoose.model('Data', DataSchema,'value');    //ชื่อ collection

// POST route to receive data from ESP32
app.post('/api/data', (req, res) => {
    const { pm1, pm2_5,  pm10 } = req.body;

    const data = new Data({ pm1, pm2_5,  pm10 });

    data.save()
        .then(() => res.status(201).send('Data saved successfully'))
        .catch(err => res.status(400).send('Error saving data'));
});

// Start the server
app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
