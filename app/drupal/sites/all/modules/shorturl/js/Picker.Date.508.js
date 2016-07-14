(function() {

var ee = 0;

	this.DatePicker508 = Picker.Date508 = new Class({
		Extends : Picker.Date,
		options : {},
				initialize : function(attachTo, options)
				{
						var self = this;

						this.parent(attachTo, options);
						if ( options.hasOwnProperty('dateFormatParser') )
						{
			    		this.options.dateFormatParser = options.dateFormatParser;
						}

						var docClickEvent = function(event)
						{
							// needs to check if any ancestor is an attached element
							if (this.toggles.contains(event.target)) { return; }
							if (this.toggles.contains(event.target.getParent())) { return; }
							this.close();
							event.stop();
						}.bind(self);

						this.reopenTabEvent = function(event)
						{
							var keyCode;
							if (window.event) {
								keyCode = window.event.keyCode;
							} else {
								keyCode = event.code;
							}

							if ( keyCode==9 )
							{
								if (!this.opened)
								{
										this.open();
										this.fireEvent('attached', [event, this.input]);
								}
							}
						}.bind(self);
						this.reopenSpaceEvent = function(event)
						{
							var keyCode;
							if (window.event) {
								keyCode = window.event.keyCode;
							} else {
								keyCode = event.code;
							}

							if ( keyCode==32 )
							{
								if (!this.opened && !this.spamming)
								{
										this.open();
										this.fireEvent('attached', [event, this.input]);
								}
							}
						}.bind(self);

						this.changeEvent = function(event)
						{console.log('change')
								var val  = this.input.get('value');
								var date = Date.parse(val);
								console.log('parsed',date);
								if (date == null || !date.isValid()) { return; }
								date = this.limitDate( date );
								this.date = date;
								var formatted = date.format(this.options.format);
								this.input.prevVal = formatted;
								var time = date.strftime();
								this.input.set('value', formatted).store('datepicker:value', time);
						}.bind(self);

						this.addEvent('open', function()
						{
		        		this.picker.getDocument().removeEvents('click')
								                         .addEvent('click', docClickEvent);
								if ( this.toggles )
								{
										this.toggles[0].addClass('opened');
										this.toggles[0].removeEvent( 'keydown', this.navEvent       )
										               .addEvent(    'keydown', this.navEvent       );
										this.toggles[0].removeEvent( 'keyup',   this.reopenTabEvent    );
										this.toggles[0].removeEvent( 'keydown', this.reopenSpaceEvent  );
								}
								if ( this.input )
								{
										this.position(this.input);
										this.input.addClass('opened');
								}
						}.bind(self) );


						this.addEvent('attached', function()
						{
								if ( this.input )
								{
										this.position(this.input);
										this.input.removeEvent( 'keyup',   this.editEvent   )
											        .addEvent(    'keyup',   this.editEvent   );
										this.input.removeEvent( 'change',  this.changeEvent )
										          .addEvent(    'change',  this.changeEvent );
										this.input.prevVal = this.input.get('value');
								}
						}.bind(self) );

						this.addEvent('close', function()
						{
		        		this.picker.getDocument().removeEvents('click');
								if ( this.toggles )
								{
										this.toggles[0].removeClass('opened');
										this.toggles[0].removeEvent( 'blur',    this.blurEvent  );
										this.toggles[0].removeEvent( 'keydown', this.navEvent   );
										this.toggles[0].removeEvent( 'keyup', this.reopenTabEvent )
										 							 .addEvent(    'keyup', this.reopenTabEvent );
										this.toggles[0].removeEvent( 'keydown', this.reopenSpaceEvent )
																	 .addEvent(    'keydown', this.reopenSpaceEvent );

								}
								if ( this.input )
								{
										this.input.removeEvent( 'keyup',   this.editEvent );
										this.input.removeClass('opened');
								}
								this.options.selection = new Array();
						}.bind(self) );

						this.doneSpamming = function(event)
						{
							this.spamming = false;
							event.target.removeEvent('keyup',this.doneSpamming);
						}.bind(this);

						this.navEvent = function(event)
						{
							var keyCode;
							if (window.event) {
								keyCode = window.event.keyCode;
							} else {
								keyCode = event.code;
							}

							switch (keyCode) {
							case 13:
								// enter --> select date
								// (according to WAI ARIA Authoring Practices 1.0)
								// continue as spacebar
							case 32:
								// spacebar --> select date
								// (according to WAI ARIA Authoring Practices 1.0)
								event.stop();
								// cancel keyspam
								if ( this.spamming ) { return; }
								this.spamming = true;
								event.target.addEvent('keyup',this.doneSpamming);
								if (  this.options.pickOnly == 'days'
												|| !this.options.timePicker
												||  this.currentView == 'time' )
								{
										this.select(this.date);

								} else if ( this.currentView == 'years' ){
										this.renderMonths(this.date.clone(),'fade');

								} else if ( this.currentView == 'months' ){
										this.renderDays(this.date.clone(),'fade');

								} else if ( this.currentView == 'days' ){
										this.date.clearTime();
										this.renderTime(this.date.clone(), 'fade');
										var hourInput = this.picker.querySelector('.time .hour');
										if ( hourInput )
										{
												hourInput.focus();
										}
								}
								break;

							case 37:
								event.stop();
								// left arrow --> go to previous day
								// (according to WAI ARIA Authoring Practices 1.0)
								if ( this.currentView == 'days' )
								{
										var m1 = this.date.getMonth();
										this.date = this.limitDate( this.date.clone().decrement('day',1) );
										var m2 = this.date.getMonth();
										this.renderDays(this.date,(m1!=m2)?'fade':'');

								} else if ( this.currentView == 'months' ) {
										var y1 = this.date.getYear();
										this.date = this.limitDate( this.date.clone().decrement('month',1) );
										var y2 = this.date.getYear();
										this.renderMonths(this.date,(y1!=y2)?'fade':'');

								} else if ( this.currentView == 'years' ) {
										this.date = this.limitDate( this.date.clone().decrement('year',1) );
										this.renderYears(this.date);


								} else if ( this.currentView == 'time' ) {
										var hourInput = this.picker.querySelector('.time .hour');
										if ( hourInput )
										{
												var value = hourInput.get('value').toInt();
												this.date.setHours( Math.max(0,Math.min(23, (value+23)%24 )) );
												hourInput.set('value', this.date.format('%H'));
										}
								}
								break;

							case 39:
								event.stop();
								if ( this.currentView == 'days' )
								{
										// right arrow --> go to next day
										// (according to WAI ARIA Authoring Practices 1.0)
										var m1 = this.date.getMonth();
										this.date = this.limitDate( this.date.clone().increment('day',1) );
										var m2 = this.date.getMonth();
										this.renderDays(this.date.clone(),(m1!=m2)?'fade':'');

								} else if ( this.currentView == 'months' ) {
										var y1 = this.date.getYear();
										this.date = this.limitDate( this.date.clone().increment('month',1) );
										var y2 = this.date.getYear();
										this.renderMonths(this.date,(y1!=y2)?'fade':'');

								} else if ( this.currentView == 'years' ) {
										this.date = this.limitDate( this.date.clone().increment('year',1) );
										this.renderYears(this.date);

								} else if ( this.currentView == 'time' ) {
										// right arrow --> next hour
										var hourInput = this.picker.querySelector('.time .hour');
										if ( hourInput )
										{
												var value = hourInput.get('value').toInt();
												this.date.setHours( Math.max(0,Math.min(23, (value+1)%24 )) );
												hourInput.set('value', this.date.format('%H'));
										}
								}
								break;

							case 38:
								event.stop();
								if ( this.currentView == 'days' )
								{
										// up arrow + ctrl --> go to same date in previous year
										// (instead of page up + ctrl because of firefox keystrokes)
										if (event.control || event.alt) {
												this.date = this.limitDate( this.date.clone().decrement('year',1) );
												this.renderDays(this.date.clone(), 'fade');
										}
										// up arrow --> go to same day in previous week
										// (according to WAI ARIA Authoring Practices 1.0)
										else {
												var m1 = this.date.getMonth();
												this.date = this.limitDate( this.date.clone().decrement('week',1) );
												var m2 = this.date.getMonth();
												this.renderDays(this.date.clone(),(m1!=m2)?'fade':'');
										}

								} else if ( this.currentView == 'months' ) {
										var y1 = this.date.getYear();
										this.date = this.limitDate( this.date.clone().decrement('month',3) );
										var y2 = this.date.getYear();
										this.renderMonths(this.date,(y1!=y2)?'fade':'');

								} else if ( this.currentView == 'years' ) {
										this.date = this.limitDate( this.date.clone().decrement('year',4) );
										this.renderYears(this.date);

								} else if ( this.currentView == 'time' ) {
										// up arrow --> next hour
										var hourInput = this.picker.querySelector('.time .hour');
										if ( hourInput )
										{
												var value = hourInput.get('value').toInt();
												this.date.setHours( Math.max(0,Math.min(23, (value+1)%24 )) );
												hourInput.set('value', this.date.format('%H'));
										}
								}
								break;

							case 40:
								event.stop();
								if ( this.currentView == 'days' )
								{
										// down arrow + ctrl --> go to same date in next year
										// (instead of page down + ctrl because of firefox keystrokes)
										if (event.control || event.alt) {
												this.date = this.limitDate( this.date.clone().increment('year',1) );
												this.renderDays(this.date.clone(), 'fade');
										}
										// down arrow --> go to same day in next week
										// (according to WAI ARIA Authoring Practices 1.0)
										else {
												var m1 = this.date.getMonth();
												this.date = this.limitDate( this.date.clone().increment('week',1) );
												var m2 = this.date.getMonth();
												this.renderDays(this.date.clone(),(m1!=m2)?'fade':'');
										}

								} else if ( this.currentView == 'months' ) {
										var y1 = this.date.getYear();
										this.date = this.limitDate( this.date.clone().increment('month',3) );
										var y2 = this.date.getYear();
										this.renderMonths(this.date.clone(),(y1!=y2)?'fade':'');

								} else if ( this.currentView == 'years' ) {
										this.date = this.limitDate( this.date.clone().increment('year',4) );
										this.renderYears(this.date.clone());

								} else if ( this.currentView == 'time' ) {
										// down arrow --> previous hour
										var hourInput = this.picker.querySelector('.time .hour');
										if ( hourInput )
										{
												var value = hourInput.get('value').toInt();
												this.date.setHours( Math.max(0,Math.min(23, (value+23)%24 )) );
												hourInput.set('value', this.date.format('%H'));
										}
								}
								break;

							case 33:
								// page up
								// (according to WAI ARIA Authoring Practices 1.0)
								event.stop();
								if ( this.currentView == 'days' )
								{
										// page up --> go to same date in previous month
										this.date = this.limitDate( this.date.clone().decrement('month',1) );
										this.renderDays(this.date.clone(), 'fade');

								} else if ( this.currentView == 'months' ) {
										// page up --> go to same month in previous year
										this.date = this.limitDate( this.date.clone().decrement('year',1) );
										this.renderMonths(this.date.clone(),'fade');

								} else if ( this.currentView == 'years' ) {
								} else if ( this.currentView == 'time' ) {
										// page up --> jump 12 hours
										var hourInput = this.picker.querySelector('.time .hour');
										if ( hourInput )
										{
												var value = hourInput.get('value').toInt();
												this.date.setHours( Math.max(0,Math.min(23, (value+12)%24 )) );
												hourInput.set('value', this.date.format('%H'));
										}
								}
								break;

							case 34:
								// page down --> go to same date in next month
								// (according to WAI ARIA Authoring Practices 1.0)
								event.stop();
								if ( this.currentView == 'days' )
								{
										// page down --> go to same date in next month
										this.date = this.limitDate( this.date.clone().increment('month',1) );
										this.renderDays(this.date.clone(), 'fade');

								} else if ( this.currentView == 'months' ) {
										// page down --> go to same month in next year
										this.date = this.limitDate( this.date.clone().increment('year',1) );
										this.renderMonths(this.date.clone(),'fade');

								} else if ( this.currentView == 'years' ) {
								} else if ( this.currentView == 'time' ) {
										// page down --> jump 12 hours
										var hourInput = this.picker.querySelector('.time .hour');
										if ( hourInput )
										{
												var value = hourInput.get('value').toInt();
												this.date.setHours( Math.max(0,Math.min(23, (value+12)%24 )) );
												hourInput.set('value', this.date.format('%H'));
										}
								}
								break;

							case 36:
								// Home --> go to original starting date
								// (according to WAI ARIA Authoring Practices 1.0)
								event.stop();
								if (event.control || event.alt) {
										this.getInputDate(this.input);
										this.renderDays(this.date.clone(), 'fade');
								} else if ( this.currentView == 'days' ) {
										this.date = this.limitDate( this.date.clone().set('date',1) );
										this.renderDays(this.date.clone());
								}
								break;

							case 35:
								// End --> go to last day of current month
								// (according to WAI ARIA Authoring Practices 1.0)
								event.stop();
								if ( this.currentView == 'days' )
								{
										this.date = this.limitDate( this.date.clone().set('date', this.date.get('lastdayofmonth')) );
										this.renderDays(this.date.clone());
								}
								break;

							case 27:
								// Escape --> close date picker without any action
								// (according to WAI ARIA Authoring Practices 1.0)
								event.stop();
								this.close();
								break;

							case 9:
								// Tab --> close date picker without any action and move the focus
								// (according to WAI ARIA Authoring Practices 1.0)
								this.close();
								break;
							}
						}.bind(this);

						this.editEvent = function(event)
						{console.log('edit')
								var currVal  = this.input.get('value');
								var currDate = Date.parse(currVal);
								if (currDate == null || !currDate.isValid()) { return; }
								if ( !this.input.prevVal )
								{
										this.input.prevVal = currVal;
								}
								this.input.prevVal = currVal;
								this.date = this.limitDate( currDate );
								this.select(this.date);
						}.bind(this);

						this.stopSubmit = function(event)
						{
							var keyCode;
							if (window.event) {
									keyCode = window.event.keyCode;
							} else {
									keyCode = event.code;
							}
							if ( keyCode==13 )
							{
								event.stop();
								return;
							}
						}.bind(self);

						this.picker.getDocument().removeEvents('click');
						options.toggle[0].setAttribute('tabIndex', '0');
						options.toggle[0].addEvent( 'keyup',   this.reopenTabEvent   );
						options.toggle[0].addEvent( 'keydown', this.reopenSpaceEvent );

						this.attach(attachTo,options.toggle);
						this.fireEvent('attached', [null,attachTo[0]]);
						this.close();
				},
				constructPicker : function()
				{
						this.parent();
						this.next.setProperty('role', 'button')
										 .setProperty('aria-label', 'Advance to next month');
						this.previous.setProperty('role', 'button')
												 .setProperty('aria-label', 'Go back to the previous month');
				},
				limitDate : function(date)
				{
					if (this.options.minDate && date < this.options.minDate) return this.options.minDate.clone();
					if (this.options.maxDate && date > this.options.maxDate) return this.options.maxDate.clone();
					return date.clone();
				},

	});

})();
