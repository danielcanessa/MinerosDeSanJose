var mongoose = require('mongoose');
var Schema = mongoose.Schema;

// create a schema
var stepSchema = new Schema({
    gas_level: {
        type: Number,
        required: [true, 'All steps should have gas_level.']
    },
    direction: {
    	   type: String,
    	 enum: ['forward', 'backward', 'left', 'right'],
    	 default: 'forward',
    	 required: [true, 'All steps should have direction.']
    }
},
{
    timestamps: true
}
);

// the schema is useless so far
// we need to create a model using it
var Step = mongoose.model('Step', stepSchema);

// make this available to our users in our Node applications
module.exports = Step;