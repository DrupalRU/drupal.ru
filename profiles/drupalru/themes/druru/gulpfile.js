var gulp          = require('gulp');
var less          = require('gulp-less');
var header        = require('gulp-header');
var rename        = require('gulp-rename');
var lessGlob      = require('gulp-less-glob');
var sourcemaps    = require('gulp-sourcemaps');
var browserSync   = require('browser-sync');
var autoprefixer  = require('gulp-autoprefixer');

// Replace 'proxy' value with actual url to your local drupal.ru
var config = {
  proxy: 'http://drupalru.lndo.site'
}

// Compile LESS with autoprefixing and create sourcemaps
gulp.task('css', function() {
  return gulp
    .src('./less/style.less')
    .pipe(sourcemaps.init())
    .pipe(lessGlob())
    .pipe(less()).on('error', console.log.bind(console))
    .pipe(autoprefixer())
    .pipe(sourcemaps.write('./'))
    .pipe(gulp.dest('./css'))
    .pipe(browserSync.reload({
      stream: true
    }));
});

// Task that ensures the 'css' task is complete before reloading browsers
gulp.task('css-watch', ['css'], function (done) {
  browserSync.reload();
  done();
});

// Default task
gulp.task('default', ['css']);

// Development task to launch Browsersync and watch CSS files
gulp.task('dev', ['css'], function () {
  browserSync.init({
    proxy: config.proxy,
    options: {
      injectChanges: false
    },
  });

  // Add browserSync.reload to the tasks array to make
  // all browsers reload after tasks are complete.
  gulp.watch('./less/**/*.less', ['css-watch']);
});